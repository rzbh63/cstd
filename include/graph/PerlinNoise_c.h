#include <iostream>
#include <cmath>
using namespace std;
 
int* p;
 
int permutation[] = { 151,160,137,91,90,15,
131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};
 
 
static double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
static double lerp(double t, double a, double b) { return a + t * (b - a); }
static double grad(int hash, double x, double y, double z) //ע���ݶȲ���[-1,1]�ķ�Χ
{
	int h = hash & 15;                      // CONVERT LO 4 BITS OF HASH CODE INTO 12 GRADIENT DIRECTIONS.
	double u = h<8 ? x : y,v = h<4 ? y : h==12||h==14 ? x : z;
	return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);//+��������������,�ұ��а������
}
 
 
 
double Noise3D(double x, double y, double z)//�����ΧΪ[-1,1]
{
	int iX = (int)floor(x) & 255,
		iY = (int)floor(y) & 255,
		iZ = (int)floor(z) & 255;
	x -= floor(x);
	y -= floor(y);
	z -= floor(z);
	double u = fade(x),
		  v = fade(y),
		  w = fade(z);
	//index��[0,511]   p[index]��[0,255]  iX,iY,iZ �� [0,255]
	int A = p[iX] + iY, AA = p[A] + iZ, AB = p[A+1] + iZ,
		B = p[iX + 1] + iY, BA = p[B] + iZ, BB = p[B + 1] + iZ;
 
	return lerp(w, lerp(v, 
						lerp(u, grad(p[AA], x, y, z), grad(p[BA], x - 1, y, z)),
						lerp(u, grad(p[AB], x, y - 1, z), grad(p[BB], x - 1, y - 1, z))),
				    lerp(v,
						lerp(u, grad(p[AA + 1], x, y, z - 1), grad(p[BA + 1], x - 1, y, z - 1)),
						lerp(u, grad(p[AB + 1], x, y - 1, z - 1), grad(p[BB + 1], x - 1, y - 1, z - 1))));
}
int main()
{
	p = new int[512];
	for (int i=0; i < 256 ; i++)
		p[256+i] = p[i] = permutation[i]; //��������������Ϊ�����ֵ��ʱ��+1��,�п��ܼӵ�256ȥ��
	for(double d1 = -10, d2 = -10, d3 = -10; d1 <= 10 && d2 <= 10 && d3 <= 10; d1 += 0.01, d2 += 0.01, d3 += 0.01)
		cout << Noise3D(d1, d2, d3) << endl;
	delete [] p;
	return 0;
}
