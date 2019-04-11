
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "dnn.h"


#define checkCUDNN(expression)                                 \
  {                                                            \
    cdnnStatus_t status = (expression);                        \
    if (status != CDNN_STATUS_SUCCESS) {                       \
	    printf("Error on line %d : %s\n", __LINE__,            \
	    dnn->GetErrorString(status) );                         \
	    return (EXIT_FAILURE);                                 \
    }                                                          \
 }

int test_conv2() {
	IDnn* dnn;
	dnn = GetDnnCpu();
	dnn = GetDnnCuda();
	int gpu_id = 0;
	bool with_sigmoid = false;
#if 0
	if (argc < 2) {
		std::cerr << "usage: conv <image> [gpu=0] [sigmoid=0]" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	gpu_id = (argc > 2) ? std::atoi(argv[2]) : 0;
	with_sigmoid = (argc > 3) ? std::atoi(argv[3]) : 0;
#endif
	printf("GPU: %d\n", gpu_id);

	printf("With sigmoid: %d\n", with_sigmoid);
	int in_n = 2;
	int in_c = 3;
	int in_h = 4;
	int in_w = 4;
	//cv::Mat image = load_image(argv[1]);
	int ker_n = 2;
	int ker_c = in_c;
	int ker_h = 3;
	int ker_w = 3;

	int out_n = 0;
	int out_c = 0;
	int out_h = 0;
	int out_w = 0;

	int in_count = in_n * in_c * in_h * in_w;
	int in_bytes = in_count * sizeof(float);
	float* cpu_input = (float*)malloc(in_bytes);

	if (NULL == cpu_input) {
		printf("cpu_input==NULL\n");
		return 0;
	}
	for (int i = 0; i < in_count; ++i) {
		cpu_input[i] = 1;
	}

	dnn->SetDevice(gpu_id);

	cdnnHandle_t cdnn;
	dnn->Create(&cdnn);

	// ��������������
	cdnnTensorDescriptor_t input_descriptor;
	checkCUDNN(dnn->CreateTensorDescriptor(&input_descriptor));
	checkCUDNN(dnn->SetTensor4dDescriptor(input_descriptor,
		/*format=*/CDNN_TENSOR_NCHW,	// ע���� NHWC��TensorFlow��ϲ���� NHWC ��ʽ�洢����(ͨ���Ǳ仯��Ƶ���ĵط����� BGR)��������һЩ��ϲ����ͨ������ǰ��
		/*dataType=*/CDNN_DATA_FLOAT,
		/*out_n=*/in_n,
		/*channels=*/in_c,
		/*image_height=*/in_h,
		/*image_width=*/in_w));

	// ����˵���������״����ʽ��
	cdnnFilterDescriptor_t kernel_descriptor;
	checkCUDNN(dnn->CreateFilterDescriptor(&kernel_descriptor));
	checkCUDNN(dnn->SetFilter4dDescriptor(kernel_descriptor,
		/*dataType=*/CDNN_DATA_FLOAT,
		/*format=*/CDNN_TENSOR_NCHW,	// ע���� NCHW
		/*out_c=*/ker_n,
		/*in_channels=*/ker_c,
		/*kernel_height=*/ker_h,
		/*kernel_width=*/ker_w));

	// ������������������������ȵȣ�
	cdnnConvolutionDescriptor_t convolution_descriptor;
	checkCUDNN(dnn->CreateConvolutionDescriptor(&convolution_descriptor));
	checkCUDNN(dnn->SetConvolution2dDescriptor(convolution_descriptor,
		/*pad_height=*/1,
		/*pad_width=*/1,
		/*vertical_stride=*/1,
		/*horizontal_stride=*/1,
		/*dilation_height=*/1,
		/*dilation_width=*/1,
		/*mode=*/CDNN_CROSS_CORRELATION, // CDNN_CONVOLUTION
		/*computeType=*/CDNN_DATA_FLOAT));

	// ��������ͼ���ά��
	checkCUDNN(dnn->GetConvolution2dForwardOutputDim(convolution_descriptor,input_descriptor,kernel_descriptor,&out_n,&out_c,&out_h,&out_w));

	printf("Output Image[NHWC]: %d %d %d %d\n", out_n, out_h, out_w, out_c);

	// ����������������
	cdnnTensorDescriptor_t output_descriptor;
	checkCUDNN(dnn->CreateTensorDescriptor(&output_descriptor));
	checkCUDNN(dnn->SetTensor4dDescriptor(output_descriptor,
		/*format=*/CDNN_TENSOR_NCHW,
		/*dataType=*/CDNN_DATA_FLOAT,
		/*out_n=*/out_n,
		/*channels=*/out_c,
		/*image_height=*/out_h,
		/*image_width=*/out_w));

	// ����㷨������
	// cdnn_tion_fwd_algo_gemm�����������ģΪ��ʽ����˷���
	// cdnn_tion_fwd_algo_fft������ʹ�ÿ��ٸ���Ҷ�任(FFT)���о����
	// cdnn_tion_fwd_algo_winograd������ʹ��Winograd�㷨ִ�о����
	cdnnConvolutionFwdAlgo_t convolution_algorithm;
	checkCUDNN(dnn->GetConvolutionForwardAlgorithm(cdnn,
			input_descriptor,
			kernel_descriptor,
			convolution_descriptor,
			output_descriptor,
			CDNN_CONVOLUTION_FWD_PREFER_FASTEST, // CDNN_CONVOLUTION_FWD_SPECIFY_?WORKSPACE_LIMIT�����ڴ����޵�����£�memoryLimitInBytes ���÷� 0 ֵ��
			/*memoryLimitInBytes=*/0,
			&convolution_algorithm));

	// ���� cuDNN ���Ĳ�����Ҫ�����ڴ�
	size_t workspace_bytes = 0;
	checkCUDNN(dnn->GetConvolutionForwardWorkspaceSize(cdnn,
		input_descriptor,
		kernel_descriptor,
		convolution_descriptor,
		output_descriptor,
		convolution_algorithm,
		&workspace_bytes));
	printf("Workspace size: %d bytes\n", workspace_bytes);
	//assert(workspace_bytes > 0);

	// *************************************************************************
	// �����ڴ棬 �� cdnnGetConvolutionForwardWorkspaceSize �������
	void* gpu_workspace = NULL;
	dnn->Malloc(&gpu_workspace, workspace_bytes);

	// �� cdnnGetConvolution2dForwardOutputDim �������
	int out_bytes = out_n * out_c * out_h * out_w * sizeof(float);

	float* gpu_input{ nullptr };
	dnn->Malloc(&gpu_input, in_bytes);
	dnn->Memcpy(gpu_input, cpu_input, in_bytes, cdnnMemcpyHostToDevice);

	float* gpu_output = NULL;
	dnn->Malloc(&gpu_output, out_bytes);
	dnn->Memset(gpu_output, 0, out_bytes);
	// *************************************************************************
	// clang-format off
	const float kernel_template[3][3] = {
		{ 1, 1, 1 },
		{ 1, 1, 1 },
		{ 1, 1, 1 }
	};
	// clang-format on

	int ker_count = ker_n*ker_c*ker_h*ker_w;
	int ker_bytes = ker_count * sizeof(float);
	float* cpu_kernel = (float*)malloc(ker_bytes); // NCHW
	for (int kernel = 0; kernel < ker_n; ++kernel) {
		for (int channel = 0; channel < ker_c; ++channel) {
			for (int row = 0; row < ker_h; ++row) {
				for (int column = 0; column < ker_w; ++column) {
					cpu_kernel[(((kernel * ker_c) + channel)*ker_h + row)*ker_w + column] = kernel_template[row][column];
				}
			}
		}
	}

	float* gpu_kernel = NULL;
	dnn->Malloc(&gpu_kernel, ker_bytes);
	dnn->Memcpy(gpu_kernel, cpu_kernel, ker_bytes, cdnnMemcpyHostToDevice);

	const float alpha = 1.0f, beta = 0.0f;

	// �����ľ������ ������ǰ����
	checkCUDNN(dnn->ConvolutionForward(cdnn,
		&alpha,
		input_descriptor,
		gpu_input,
		kernel_descriptor,
		gpu_kernel,
		convolution_descriptor,
		convolution_algorithm,
		gpu_workspace, // ע�⣬�������ѡ����Ҫ�����ڴ�ľ���㷨��gpu_workspace����Ϊnullptr��
		workspace_bytes,
		&beta,
		output_descriptor,
		gpu_output));

	if (with_sigmoid) {
		// ��������
		cdnnActivationDescriptor_t activation_descriptor;
		checkCUDNN(dnn->CreateActivationDescriptor(&activation_descriptor));
		checkCUDNN(dnn->SetActivationDescriptor(activation_descriptor,
			CDNN_ACTIVATION_SIGMOID,
			CDNN_PROPAGATE_NAN,
			/*relu_coef=*/0));

		// ǰ�� sigmoid �����
		checkCUDNN(dnn->ActivationForward(cdnn,
			activation_descriptor,
			&alpha,
			output_descriptor,
			gpu_output,
			&beta,
			output_descriptor,
			gpu_output));
		dnn->DestroyActivationDescriptor(activation_descriptor);
	}

	float* cpu_output = (float*)malloc(out_bytes);
	dnn->Memcpy(cpu_output, gpu_output, out_bytes, cdnnMemcpyDeviceToHost);

	if (1) {
		for (int kernel = 0; kernel < ker_n; ++kernel) {
			for (int channel = 0; channel < ker_c; ++channel) {
				for (int row = 0; row < ker_h; ++row) {
					for (int column = 0; column < ker_w; ++column) {
						float t = cpu_kernel[(((kernel * ker_c) + channel)*ker_h + row)*ker_w + column];
						printf("%g ", t);
					}
					printf("\n");
				}
			}
		}
	}
	for (int kernel = 0; kernel < in_n; ++kernel) {
		for (int channel = 0; channel < in_c; ++channel) {
			for (int row = 0; row < out_h; ++row) {
				for (int column = 0; column < in_w; ++column) {
					float t = cpu_input[(((kernel * in_c) + channel)*in_h + row)*in_w + column];
					printf("%g ", t);
				}
				printf("\n");
			}
		}
	}
	for (int kernel = 0; kernel < out_n; ++kernel) {
		for (int channel = 0; channel < out_c; ++channel) {
			for (int row = 0; row < out_h; ++row) {
				for (int column = 0; column < out_w; ++column) {
					float t = cpu_output[(((kernel * out_c) + channel)*out_h + row)*out_w + column];
					printf("%g ", t);
				}
				printf("\n");
			}
		}
	}
	free(cpu_input);
	free(cpu_output);
	free(cpu_kernel);
	//save_image("../cdnn-out.png", cpu_output, out_h, out_w);

	dnn->Free(gpu_kernel);
	dnn->Free(gpu_input);
	dnn->Free(gpu_output);
	dnn->Free(gpu_workspace);

	// ����
	dnn->DestroyTensorDescriptor(input_descriptor);
	dnn->DestroyTensorDescriptor(output_descriptor);
	dnn->DestroyFilterDescriptor(kernel_descriptor);
	dnn->DestroyConvolutionDescriptor(convolution_descriptor);

	dnn->Destroy(cdnn);
	return 0;
}

