
# convert vgg-face model weight from caffe to pytorch - 机器学习的小学生 - CSDN博客


2018年05月02日 21:27:46[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：554



```python
#coding=UTF-8
```
```python
import
```
```python
torch
```
```python
import
```
```python
torch.nn
```
```python
as
```
```python
nn
```
```python
import
```
```python
math
```
```python
import
```
```python
numpy
```
```python
as
```
```python
np
```
```python
from
```
```python
PIL
```
```python
import
```
```python
Image,ImageDraw
```
```python
import
```
```python
matplotlib.pyplot
```
```python
as
```
```python
plt
```
```python
import
```
```python
collections
```
```python
import
```
```python
matplotlib.cm
```
```python
as
```
```python
cm
```
```python
from
```
```python
torch.autograd
```
```python
import
```
```python
Variable
```
```python
from
```
```python
torchvision
```
```python
import
```
```python
models
```
```python
import
```
```python
caffe
```
```python
def
```
```python
vis_square
```
```python
(data)
```
```python
:
```
```python
"""Take an array of shape (n, height, width) or (n, height, width, 3)
       and visualize each (height, width) thing in a grid of size approx. sqrt(n) by sqrt(n)"""
```
```python
# normalize data for display
```
```python
data = (data - data.min()) / (data.max() - data.min())
```
```python
# force the number of filters to be square
```
```python
n = int(np.ceil(np.sqrt(data.shape[
```
```python
0
```
```python
])))
    padding = (((
```
```python
0
```
```python
, n **
```
```python
2
```
```python
- data.shape[
```
```python
0
```
```python
]),
               (
```
```python
0
```
```python
,
```
```python
1
```
```python
), (
```
```python
0
```
```python
,
```
```python
1
```
```python
))
```
```python
# add some space between filters
```
```python
+ ((
```
```python
0
```
```python
,
```
```python
0
```
```python
),) * (data.ndim -
```
```python
3
```
```python
))
```
```python
# don't pad the last dimension (if there is one)
```
```python
data = np.pad(data, padding, mode=
```
```python
'constant'
```
```python
, constant_values=
```
```python
1
```
```python
)
```
```python
# pad with ones (white)
```
```python
# tile the filters into an image
```
```python
data = data.reshape((n, n) + data.shape[
```
```python
1
```
```python
:]).transpose((
```
```python
0
```
```python
,
```
```python
2
```
```python
,
```
```python
1
```
```python
,
```
```python
3
```
```python
) + tuple(range(
```
```python
4
```
```python
, data.ndim +
```
```python
1
```
```python
)))
    data = data.reshape((n * data.shape[
```
```python
1
```
```python
], n * data.shape[
```
```python
3
```
```python
]) + data.shape[
```
```python
4
```
```python
:])
    plt.imshow(data); plt.axis(
```
```python
'off'
```
```python
)
```
```python
def
```
```python
vis_activation
```
```python
(activations, img_arr)
```
```python
:
```
```python
assert
```
```python
(len(activations.shape) ==
```
```python
3
```
```python
),
```
```python
'make sure : nchannels * height * width '
```
```python
n_nodes = activations.shape[
```
```python
0
```
```python
]
```
```python
# number of nodels
```
```python
print(
```
```python
'n_nodes: '
```
```python
,n_nodes)
    vis_size = activations.shape[
```
```python
1
```
```python
:]
```
```python
#visualization shape
```
```python
print(
```
```python
'visual_size: '
```
```python
,vis_size)
    vis = np.zeros((vis_size[
```
```python
0
```
```python
], vis_size[
```
```python
1
```
```python
]), dtype=np.float32)
```
```python
#generating saliency image
```
```python
for
```
```python
i
```
```python
in
```
```python
range(n_nodes):
        activation = activations[i, :, :]
        weight =
```
```python
1
```
```python
weighted_activation = activation*weight
        vis += weighted_activation
    vis = np.maximum(vis,
```
```python
0
```
```python
)
```
```python
# relu
```
```python
vis_img = Image.fromarray(vis,
```
```python
None
```
```python
)
    vis_img = vis_img.resize((
```
```python
224
```
```python
,
```
```python
224
```
```python
),Image.BICUBIC)
    vis_img = vis_img / np.max(vis_img)

    vis_img = Image.fromarray(np.uint8(cm.jet(vis_img) *
```
```python
255
```
```python
))
```
```python
# winter ,jet
```
```python
vis_img = vis_img.convert(
```
```python
'RGB'
```
```python
)
```
```python
# dropping alpha channel
```
```python
input_image = Image.fromarray(img_arr)
    input_image = input_image.resize((
```
```python
224
```
```python
,
```
```python
224
```
```python
))
    input_image = input_image.convert(
```
```python
'RGB'
```
```python
)
    plt.imshow(vis_img)
```
```python
#plt.show()
```
```python
#print vis_img.size, input_i
```
```python
heat_map = Image.blend(input_image, vis_img,
```
```python
0.5
```
```python
)
    plt.imshow(heat_map)
    plt.axis(
```
```python
'off'
```
```python
)
```
```python
class
```
```python
vgg16
```
```python
(nn.Module)
```
```python
:
```
```python
def
```
```python
__init__
```
```python
(self,num_classes=
```
```python
1000
```
```python
)
```
```python
:
```
```python
super(vgg16,self).__init__()
```
```python
# call  parents' init method'
```
```python
inplace =
```
```python
True
```
```python
self.features = nn.Sequential(
            nn.Conv2d(
```
```python
3
```
```python
,
```
```python
64
```
```python
,kernel_size=(
```
```python
3
```
```python
,
```
```python
3
```
```python
),stride=(
```
```python
1
```
```python
,
```
```python
1
```
```python
),padding=(
```
```python
1
```
```python
,
```
```python
1
```
```python
)),
```
```python
# input_channel, output_channel
```
```python
nn.ReLU(inplace),
            nn.Conv2d(
```
```python
64
```
```python
,
```
```python
64
```
```python
, kernel_size=(
```
```python
3
```
```python
,
```
```python
3
```
```python
), stride=(
```
```python
1
```
```python
,
```
```python
1
```
```python
), padding=(
```
```python
1
```
```python
,
```
```python
1
```
```python
)),
            nn.ReLU(inplace),
            nn.MaxPool2d(kernel_size=(
```
```python
2
```
```python
,
```
```python
2
```
```python
), stride=(
```
```python
2
```
```python
,
```
```python
2
```
```python
), dilation=(
```
```python
1
```
```python
,
```
```python
1
```
```python
), ceil_mode=
```
```python
False
```
```python
),
            nn.Conv2d(
```
```python
64
```
```python
,
```
```python
128
```
```python
, kernel_size=(
```
```python
3
```
```python
,
```
```python
3
```
```python
), stride=(
```
```python
1
```
```python
,
```
```python
1
```
```python
), padding=(
```
```python
1
```
```python
,
```
```python
1
```
```python
)),
            nn.ReLU(inplace),
            nn.Conv2d(
```
```python
128
```
```python
,
```
```python
128
```
```python
, kernel_size=(
```
```python
3
```
```python
,
```
```python
3
```
```python
), stride=(
```
```python
1
```
```python
,
```
```python
1
```
```python
), padding=(
```
```python
1
```
```python
,
```
```python
1
```
```python
)),
            nn.ReLU(inplace),
            nn.MaxPool2d(kernel_size=(
```
```python
2
```
```python
,
```
```python
2
```
```python
), stride=(
```
```python
2
```
```python
,
```
```python
2
```
```python
), dilation=(
```
```python
1
```
```python
,
```
```python
1
```
```python
), ceil_mode=
```
```python
False
```
```python
),
            nn. Conv2d(
```
```python
128
```
```python
,
```
```python
256
```
```python
, kernel_size=(
```
```python
3
```
```python
,
```
```python
3
```
```python
), stride=(
```
```python
1
```
```python
,
```
```python
1
```
```python
), padding=(
```
```python
1
```
```python
,
```
```python
1
```
```python
)),
            nn.ReLU(inplace),
            nn.Conv2d(
```
```python
256
```
```python
,
```
```python
256
```
```python
, kernel_size=(
```
```python
3
```
```python
,
```
```python
3
```
```python
), stride=(
```
```python
1
```
```python
,
```
```python
1
```
```python
), padding=(
```
```python
1
```
```python
,
```
```python
1
```
```python
)),
            nn.ReLU(inplace),
            nn.Conv2d(
```
```python
256
```
```python
,
```
```python
256
```
```python
, kernel_size=(
```
```python
3
```
```python
,
```
```python
3
```
```python
), stride=(
```
```python
1
```
```python
,
```
```python
1
```
```python
), padding=(
```
```python
1
```
```python
,
```
```python
1
```
```python
)),
            nn.ReLU(inplace),
            nn.MaxPool2d(kernel_size=(
```
```python
2
```
```python
,
```
```python
2
```
```python
), stride=(
```
```python
2
```
```python
,
```
```python
2
```
```python
), dilation=(
```
```python
1
```
```python
,
```
```python
1
```
```python
), ceil_mode=
```
```python
False
```
```python
),
            nn.Conv2d(
```
```python
256
```
```python
,
```
```python
512
```
```python
, kernel_size=(
```
```python
3
```
```python
,
```
```python
3
```
```python
), stride=(
```
```python
1
```
```python
,
```
```python
1
```
```python
), padding=(
```
```python
1
```
```python
,
```
```python
1
```
```python
)),
            nn.ReLU(inplace),
            nn.Conv2d(
```
```python
512
```
```python
,
```
```python
512
```
```python
, kernel_size=(
```
```python
3
```
```python
,
```
```python
3
```
```python
), stride=(
```
```python
1
```
```python
,
```
```python
1
```
```python
), padding=(
```
```python
1
```
```python
,
```
```python
1
```
```python
)),
            nn.ReLU(inplace),
            nn.Conv2d(
```
```python
512
```
```python
,
```
```python
512
```
```python
, kernel_size=(
```
```python
3
```
```python
,
```
```python
3
```
```python
), stride=(
```
```python
1
```
```python
,
```
```python
1
```
```python
), padding=(
```
```python
1
```
```python
,
```
```python
1
```
```python
)),
            nn.ReLU(inplace),
            nn.MaxPool2d(kernel_size=(
```
```python
2
```
```python
,
```
```python
2
```
```python
), stride=(
```
```python
2
```
```python
,
```
```python
2
```
```python
), dilation=(
```
```python
1
```
```python
,
```
```python
1
```
```python
), ceil_mode=
```
```python
False
```
```python
),
            nn.Conv2d(
```
```python
512
```
```python
,
```
```python
512
```
```python
, kernel_size=(
```
```python
3
```
```python
,
```
```python
3
```
```python
), stride=(
```
```python
1
```
```python
,
```
```python
1
```
```python
), padding=(
```
```python
1
```
```python
,
```
```python
1
```
```python
)),
            nn.ReLU(inplace),
            nn.Conv2d(
```
```python
512
```
```python
,
```
```python
512
```
```python
, kernel_size=(
```
```python
3
```
```python
,
```
```python
3
```
```python
), stride=(
```
```python
1
```
```python
,
```
```python
1
```
```python
), padding=(
```
```python
1
```
```python
,
```
```python
1
```
```python
)),
            nn.ReLU(inplace),
            nn.Conv2d(
```
```python
512
```
```python
,
```
```python
512
```
```python
, kernel_size=(
```
```python
3
```
```python
,
```
```python
3
```
```python
), stride=(
```
```python
1
```
```python
,
```
```python
1
```
```python
), padding=(
```
```python
1
```
```python
,
```
```python
1
```
```python
)),
            nn.ReLU(inplace),
            nn.MaxPool2d(kernel_size=(
```
```python
2
```
```python
,
```
```python
2
```
```python
), stride=(
```
```python
2
```
```python
,
```
```python
2
```
```python
), dilation=(
```
```python
1
```
```python
,
```
```python
1
```
```python
), ceil_mode=
```
```python
False
```
```python
)  
            )
        self.classifier = nn.Sequential(
            nn.Linear(in_features=
```
```python
25088
```
```python
, out_features=
```
```python
4096
```
```python
, bias=
```
```python
True
```
```python
),
            nn.ReLU(inplace),
            nn.Dropout(p=
```
```python
0.5
```
```python
),
            nn.Linear(in_features=
```
```python
4096
```
```python
, out_features=
```
```python
4096
```
```python
, bias=
```
```python
True
```
```python
),
            nn.ReLU(inplace),
            nn.Dropout(p=
```
```python
0.5
```
```python
),
            nn.Linear(in_features=
```
```python
4096
```
```python
, out_features=num_classes, bias=
```
```python
True
```
```python
)
            )
        self._initialize_weights()
```
```python
def
```
```python
forward
```
```python
(self,x)
```
```python
:
```
```python
x = self.features(x)
        x = x.view(x.size(
```
```python
0
```
```python
),-
```
```python
1
```
```python
)
        x = self.classifier(x)
```
```python
return
```
```python
x
```
```python
def
```
```python
_initialize_weights
```
```python
(self)
```
```python
:
```
```python
for
```
```python
m
```
```python
in
```
```python
self.modules():
```
```python
if
```
```python
isinstance(m, nn.Conv2d):
                n = m.kernel_size[
```
```python
0
```
```python
] * m.kernel_size[
```
```python
1
```
```python
] * m.out_channels
                m.weight.data.normal_(
```
```python
0
```
```python
, math.sqrt(
```
```python
2.
```
```python
/ n))
```
```python
if
```
```python
m.bias
```
```python
is
```
```python
not
```
```python
None
```
```python
:
                    m.bias.data.zero_()
```
```python
elif
```
```python
isinstance(m, nn.BatchNorm2d):
                m.weight.data.fill_(
```
```python
1
```
```python
)
                m.bias.data.zero_()
```
```python
elif
```
```python
isinstance(m, nn.Linear):
                m.weight.data.normal_(
```
```python
0
```
```python
,
```
```python
0.01
```
```python
)
                m.bias.data.zero_()
model = vgg16()
```
```python
# print items
```
```python
#print(model)
```
```python
params = model.state_dict()
print(type(params))
```
```python
for
```
```python
key,item
```
```python
in
```
```python
params.items():
    print(key,
```
```python
': '
```
```python
, type(item))
```
```python
# tensor
```
```python
filepath =
```
```python
'../my_headpose_using_VGG_version2/pretrained_model/vgg_face_caffe/'
```
```python
pretrained_weight = filepath +
```
```python
'VGG_FACE.caffemodel'
```
```python
deploy_pro = filepath +
```
```python
'VGG_FACE_deploy.prototxt'
```
```python
net = caffe.Net(deploy_pro,pretrained_weight,caffe.TEST)
print(
```
```python
'load caffemodel over ....'
```
```python
)
```
```python
'''
orderdict = collections.OrderedDict()
orderdict['features.0'] = 'conv1_1'
orderdict['features.2'] = 'conv1_2'
orderdict['features.5'] = 'conv2_1'
orderdict['features.7'] = 'conv2_2'
orderdict['features.10'] = 'conv3_1'
orderdict['features.12'] = 'conv3_2'
orderdict['features.14'] = 'conv3_3'
orderdict['features.17'] = 'conv4_1'
orderdict['features.19'] = 'conv4_2'
orderdict['features.21'] = 'conv4_3'
orderdict['features.24'] = 'conv5_1'
orderdict['features.26'] = 'conv5_2'
orderdict['features.28'] = 'conv5_3'
orderdict['classifier.0'] = 'fc6'
orderdict['classifier.3'] = 'fc7'
orderdict['classifier.6'] = 'fc8'
'''
```
```python
dict_feature = {
```
```python
0
```
```python
:
```
```python
'conv1_1'
```
```python
,
```
```python
2
```
```python
:
```
```python
'conv1_2'
```
```python
,
```
```python
5
```
```python
:
```
```python
'conv2_1'
```
```python
,
```
```python
7
```
```python
:
```
```python
'conv2_2'
```
```python
,
```
```python
10
```
```python
:
```
```python
'conv3_1'
```
```python
,
```
```python
12
```
```python
:
```
```python
'conv3_2'
```
```python
,
```
```python
14
```
```python
:
```
```python
'conv3_3'
```
```python
,
```
```python
17
```
```python
:
```
```python
'conv4_1'
```
```python
,
```
```python
19
```
```python
:
```
```python
'conv4_2'
```
```python
,
```
```python
21
```
```python
:
```
```python
'conv4_3'
```
```python
,
```
```python
24
```
```python
:
```
```python
'conv5_1'
```
```python
,
```
```python
26
```
```python
:
```
```python
'conv5_2'
```
```python
,
```
```python
28
```
```python
:
```
```python
'conv5_3'
```
```python
}
dict_classifier = {
```
```python
0
```
```python
:
```
```python
'fc6'
```
```python
,
```
```python
3
```
```python
:
```
```python
'fc7'
```
```python
}
```
```python
#                   6:'fc8'}
```
```python
for
```
```python
i
```
```python
in
```
```python
dict_feature:
    caffe_weight = net.params[dict_feature[i]][
```
```python
0
```
```python
].data
```
```python
#print(type(model.features[i].weight))
```
```python
caffe_weight = torch.from_numpy(caffe_weight).view_as(model.features[i].weight)
    model.features[i].weight.data.copy_(caffe_weight)
    model.features[i].bias.data.copy_(torch.from_numpy(np.array(net.params[dict_feature[i]][
```
```python
1
```
```python
].data)))
```
```python
for
```
```python
i
```
```python
in
```
```python
dict_classifier:
    model.classifier[i].weight.data.copy_(torch.from_numpy(np.array(net.params[dict_classifier[i]][
```
```python
0
```
```python
].data)))
    model.classifier[i].bias.data.copy_(torch.from_numpy(np.array(net.params[dict_classifier[i]][
```
```python
1
```
```python
].data)))
print(
```
```python
'copy weight from caffemodel to pytorch over....'
```
```python
)
```
```python
########## check #####################
```
```python
imgSize = [
```
```python
224
```
```python
,
```
```python
224
```
```python
]
imgpath =
```
```python
'../copy_caffemodel_2_pytorch/cat.jpg'
```
```python
img = Image.open(imgpath)
res_img = img.resize((imgSize[
```
```python
0
```
```python
],imgSize[
```
```python
1
```
```python
]))
img = np.double(res_img)
temp_img = np.uint8(res_img)
```
```python
# for vis
```
```python
img = img[:,:,(
```
```python
2
```
```python
,
```
```python
1
```
```python
,
```
```python
0
```
```python
)]
```
```python
# rgb 2 bgr
```
```python
img = np.transpose(img, (
```
```python
2
```
```python
,
```
```python
0
```
```python
,
```
```python
1
```
```python
))
```
```python
# h * w *c==> c*h*w
```
```python
print(img.shape)
data_arr = np.zeros(shape=(
```
```python
1
```
```python
,
```
```python
3
```
```python
,imgSize[
```
```python
0
```
```python
],imgSize[
```
```python
1
```
```python
]),dtype=np.float32)
data_arr[
```
```python
0
```
```python
,...] = img
input_data = Variable(torch.from_numpy(data_arr).type(torch.FloatTensor))
feat_result  = []
```
```python
def
```
```python
get_features_hook
```
```python
(self,input,output)
```
```python
:
```
```python
# number of input:
```
```python
print(
```
```python
'len(input): '
```
```python
,len(input))
```
```python
# number of output:
```
```python
print(
```
```python
'len(output): '
```
```python
,len(output))
    print(
```
```python
'###################################'
```
```python
)
    print(input[
```
```python
0
```
```python
].shape)
```
```python
# torch.Size([1, 3, 224, 224])
```
```python
print(
```
```python
'###################################'
```
```python
)
    print(output[
```
```python
0
```
```python
].shape)
```
```python
#
```
```python
feat_result.append(output.data.cpu().numpy())
handle_feat = model.features[
```
```python
0
```
```python
].register_forward_hook(get_features_hook)
```
```python
# conv1_1
```
```python
handle_heat = model.features[
```
```python
30
```
```python
].register_forward_hook(get_features_hook)
```
```python
# pool5
```
```python
handle_fc7 = model.classifier[
```
```python
3
```
```python
].register_forward_hook(get_features_hook)
```
```python
# fc7
```
```python
model.eval()
```
```python
# make dropout invalid in test stage
```
```python
score  = model(input_data)
handle_feat.remove()
handle_heat.remove()
feat1 = feat_result[
```
```python
0
```
```python
]
feat1_heat = feat_result[
```
```python
1
```
```python
]

vis_square(feat1[
```
```python
0
```
```python
,...])
```
```python
#plt.show()
```
```python
plt.savefig(
```
```python
'feat_visual_pytorch.png'
```
```python
)
```
```python
####################################### pytorch heatmap ####################################
```
```python
vis_activation(feat1_heat[
```
```python
0
```
```python
,...],temp_img)
print(
```
```python
'show heatmap for pytorch...'
```
```python
)
```
```python
#plt.show()
```
```python
plt.savefig(
```
```python
'heatmap_visual_pytorch.png'
```
```python
)
```
```python
############################################  for caffe ############################################
```
```python
net.blobs[
```
```python
'data'
```
```python
].reshape(
```
```python
1
```
```python
,
```
```python
3
```
```python
,imgSize[
```
```python
0
```
```python
],imgSize[
```
```python
1
```
```python
])
net.blobs[
```
```python
'data'
```
```python
].data[...] = data_arr
output = net.forward()
feat2 = net.blobs[
```
```python
'conv1_1'
```
```python
].data[
```
```python
0
```
```python
,...] 
vis_square(feat2)
```
```python
#plt.show()
```
```python
plt.savefig(
```
```python
'feat_visual_caffe.png'
```
```python
)
```
```python
############################# caffe heatmap ################################
```
```python
feat2_heat = net.blobs[
```
```python
'pool5'
```
```python
].data[
```
```python
0
```
```python
,...] 
vis_activation(feat2_heat,temp_img)
```
```python
#plt.show()
```
```python
plt.savefig(
```
```python
'heatmap_visual_caffe.png'
```
```python
)
```
```python
######################### check fc7 layer#######################
```
```python
fc7_pytorch = feat_result[
```
```python
2
```
```python
]
fc7_caffe = net.blobs[
```
```python
'fc7'
```
```python
].data
print(fc7_pytorch.shape)
print(fc7_caffe.shape)
err = np.max(np.abs(fc7_pytorch - fc7_caffe))
print(err)
```
```python
#6.914139e-06
```
**验证结果图：**
下图分别是： feat_visual_pytorch.png 和 heatmap_visual_pytorch.png
对于caffe现实的结果相同，不再附图。
![这里写图片描述](https://img-blog.csdn.net/20180502212528152?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3h1bHVodWkxMjM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180502212642565?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3h1bHVodWkxMjM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
参考文献：
1.[https://github.com/marvis/pytorch-caffe](https://github.com/marvis/pytorch-caffe)

