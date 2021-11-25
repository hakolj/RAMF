// APIs of torch tensors
#pragma once
#ifndef TORCHAPI_H
#define TORCHAPI_H


#include "pch.h"
#include <torch/torch.h>

class TorchAPI {
public:
	template<typename Tvector>
	static torch::Tensor from_vector(std::vector<std::vector<Tvector>> vec) {
		int size0 = vec.size();
		int size1 = vec[0].size();
		torch::Tensor tensor = torch::zeros({ size0, size1 });
		auto accessor = tensor.accessor<float, 2>();
		for (int i = 0; i < size0; i++)
		{
			for (int j = 0; j < size1; j++)
			{
				accessor[i][j] = float(vec[i][j]);
			}
		}
		return tensor;
	}
	template<typename Tvector>
	static torch::Tensor from_vector(std::vector<Tvector> vec) {
		int size0 = vec.size();
		int size1 = 1;
		torch::Tensor tensor = torch::zeros({ size0, size1 });
		auto accessor = tensor.accessor<float, 2>();
		for (int i = 0; i < size0; i++)
		{
			accessor[i][0] = float(vec[i]);
		}
		return tensor;
	}

public:
	template<typename T>
	static std::vector<std::vector<T>> as2Dvector(torch::Tensor tensor) {
		int size0 = tensor.size(0);
		int size1 = tensor.size(1);
		std::vector<std::vector<T>> vec(size0, std::vector<T>(size1));
		auto accessor = tensor.accessor<float, 2>();
		for (int i = 0; i < size0; i++)
		{
			for (int j = 0; j < size1; j++)
			{
				vec[i][j] = accessor[i][j];
			}
		}
		return vec;
	}
};

#endif // !TORCHAPI_H