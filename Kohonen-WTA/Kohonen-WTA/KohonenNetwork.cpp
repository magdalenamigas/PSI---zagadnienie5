#include<iostream>
#include<cstdlib>
#include<fstream>
#include<list>
#include<ctime>
#include<math.h>
using namespace std;

class Neuron {

public:
	int patternSize;
	int inputSize;
	double **inputs;
	double output;
	double *weights;
	void calculateValue(int index);
	void adjustWeights(int index,int it);
	void drawWeights();
	void normalizeData(double *tab, int size);
	void printWeights();
	double learningCoefficient(int it);
	Neuron();
};

Neuron::Neuron() {

	fstream plik;
	plik.open("zbior_uczacy2.txt");
	if (plik.good())
	{
		inputSize = 4;
		patternSize = 30;
		inputs = new double*[patternSize];
		for (int i = 0;i < patternSize;i++)
			inputs[i] = new double[inputSize];

		for (int k = 0; k < patternSize; k++)
		{
			for (int i = 0; i < inputSize; i++)
			{
				inputs[k][i] = 0;
			}
		}

		weights = new double[4];
		for (int i = 0; i < 4; i++)
			weights[i] = 0;

		for (int k = 0; k < patternSize; k++)
		{
			for (int i = 0; i < inputSize; i++)
			{
				plik >> inputs[k][i];
			}
		}

	}
	else
	{
		cout << "blad otwarcia pliku!";
	}
	drawWeights();
}

void Neuron::calculateValue(int index) {

	output = 0;

	for (int i = 0; i < inputSize - 1; i++)
	{
	output +=pow( inputs[index][i]-weights[i],2);
	}
	output = 1/sqrt(output);
}

void Neuron::drawWeights() {

	double sum = 0;
	for (int i = 0; i < inputSize; i++)
	{
		weights[i] = 5 + (double)rand() / RAND_MAX* (10);
		sum += pow(weights[i], 2);
	}
}

void Neuron::normalizeData(double *tab, int size)
{
	double sum = 0;
	for (int i = 0; i < size; i++)
	{
		sum += pow(tab[i], 2);
	}
	for (int i = 0; i < size; i++)
	{
		tab[i] =( tab[i] / sqrt(sum));
	}
}

void Neuron::printWeights() {

	for (int i = 0; i < inputSize; i++)
	{
		cout << weights[i] << " ";
	}
	cout << endl;
}

void Neuron::adjustWeights(int index, int it) {

	for (int i = 0; i < 4; i++)
	{
		weights[i] = weights[i] + learningCoefficient(it)*(inputs[index][i] - weights[i]);
	}
}

double Neuron::learningCoefficient(int it) {

	return exp(-it / 1000)*0.1;
}

class KohonenNetwork {

public:
	Neuron* network;
	double N;
	void learnNetwork();
	void testNetwork(double *tab, int s);
	KohonenNetwork(double N, int size);
	int size;
};

KohonenNetwork::KohonenNetwork(double N, int size):N(N),size(size){
	
	network = new Neuron[size];		
}

void KohonenNetwork::learnNetwork() {

	int index_max = 0;
	double max = 0;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			for (int k = 0; k < size; k++)
			{
				network[k].calculateValue(j);
				if (network[k].output > max)
				{
					max = network[k].output;
					index_max = k;
				}
			}
			network[index_max].adjustWeights(j,i);
			index_max = 0;
			max = 0;
		}
	}
}

void KohonenNetwork::testNetwork(double *tab, int s) {
	
	int index_max = 0;
	double max = 0;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			network[i].output += (network[i].weights[j] * tab[j]);	
		} 
		if (network[i].output > max)
		{
			max = network[i].output;
			index_max = i;
		}
	}

	for (int i = 0; i < size; i++)
	{
		cout << i+1 << "  " << network[i].output << endl;
	}	
}
 
int main() {

	srand(time(NULL));
	double tab1[4] = { 5.1,	3.5, 1.4, 0.2 }; //1
	double tab2[4] = { 4.9,	3.0, 1.4, 0.2 }; //1
	double tab3[4] = { 7.0,	3.2, 4.7, 1.4 }; //2
	double tab4[4] = { 6.4,	3.2, 4.5, 1.5 }; //2
	double tab5[4] = { 6.3,	3.3, 6.0, 2.5 }; //3
	double tab6[4] = { 5.8,	2.7, 5.1, 1.9 }; //3
	
	KohonenNetwork net(2,3);
	net.learnNetwork();

	net.testNetwork(tab1,4);
	cout << endl;
	net.testNetwork(tab4, 4);
	cout << endl;
	net.testNetwork(tab6, 4);
	cout << endl;

	getchar();
}