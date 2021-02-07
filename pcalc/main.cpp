/**
 * Cryptocurrency neutral profit calculator.
 * Based on bitkub's market.
 */
#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <cstring>
#include <unistd.h>

// fee percentage (already applied percentage)
static constexpr double kFeePercentage = 0.0025;

inline constexpr const char* kPrefixRedTxt = "\033[1;31m";
inline constexpr const char* kPrefixGreenTxt = "\033[1;32m";
inline constexpr const char* kPostfixTxt = "\033[0m";

static const bool g_supportTTY = isatty(STDOUT_FILENO);

static bool FoundCmdOption(int argc, char* argv[], const char* option)
{
	for (int i=1; i<argc; ++i)
	{
		if (std::strcmp(argv[i], option) == 0)
		{
			return true;
		}
	}
	return false;
}

static void ComputeAndPrintProfitReferencePointEntry(double percentageMargin, double cryptoSellPrice, int strFormatWidth, double grossSellPrice, double netSellPrice, double feePercentage, double amountToSpend, bool includedGrossCompute)
{
	double actualMultiPercentage = 1.0 + percentageMargin/100.0;
	std::printf("Sell at %.2f%% more at  %.4f THB\n", percentageMargin, cryptoSellPrice*actualMultiPercentage);
	if (includedGrossCompute)
	{
		double n = (grossSellPrice*actualMultiPercentage)-amountToSpend;
		if (n < 0.0 && g_supportTTY)
			std::printf("|_ Gross profit:         %s%*.8f%s     THB\n", kPrefixRedTxt, strFormatWidth, n, kPostfixTxt);
		else if (n > 0.0 && g_supportTTY)
			std::printf("|_ Gross profit:         %s%*.8f%s     THB\n", kPrefixGreenTxt, strFormatWidth, n, kPostfixTxt);
		else
			std::printf("|_ Gross profit:         %*.8f     THB\n", strFormatWidth, n);
	}


	{
		double n = (netSellPrice*actualMultiPercentage)-(netSellPrice*actualMultiPercentage*feePercentage)-amountToSpend;
		if (n < 0.0 && g_supportTTY)
			std::printf("|_ Net profit:           %s%*.8f%s     THB\n\n", kPrefixRedTxt, strFormatWidth, n, kPostfixTxt);
		else if (n > 0.0 && g_supportTTY)
			std::printf("|_ Net profit:           %s%*.8f%s     THB\n\n", kPrefixGreenTxt, strFormatWidth, n, kPostfixTxt);
		else
			std::printf("|_ Net profit:           %*.8f     THB\n\n", strFormatWidth, n);
	}
}

// accept the target buy price, and target sell price later
// to compute the profit
int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		std::cerr << "Usage: pcalc <amount-to-spend> <CRYPTO-buy-price> <CRYPTO-sell-price>\n";
		return -1;
	}	

	// this option takes higest priority than all others if included along with other options
	if (FoundCmdOption(argc, argv, "--help"))
	{
		std::cout << "Usage: pcalc <amount-to-spend> <CRYPTO-buy-price> <CRYPTO-sell-price>\n";
		return 0;
	}

	// cmd's options
	bool isIncludeGrossCompute = false;
	bool isIncludeProfitReferencePoints = false;

	if (FoundCmdOption(argc, argv, "--include-gross"))
		isIncludeGrossCompute = true;
	if (FoundCmdOption(argc, argv, "--include-profit-ref-points") || FoundCmdOption(argc, argv, "--include-prps"))
		isIncludeProfitReferencePoints = true;

	double amountToSpend = atof(argv[1]);
	double CRYPTO_buyPrice = atof(argv[2]);
	double CRYPTO_sellPrice = atof(argv[3]);

	double buyFee = amountToSpend * kFeePercentage;
	double grossAmountGainedCRYPTO = amountToSpend / CRYPTO_buyPrice;
	double netAmountGainedCRYPTO = (amountToSpend - buyFee) / CRYPTO_buyPrice;

	double grossSellPrice = grossAmountGainedCRYPTO * CRYPTO_sellPrice;
	double sellFee = netAmountGainedCRYPTO * CRYPTO_sellPrice * kFeePercentage;
	double netSellPrice = (netAmountGainedCRYPTO * CRYPTO_sellPrice) - sellFee;
	
	double grossProfit = grossSellPrice - amountToSpend;
	double profit = netSellPrice - amountToSpend;

	constexpr int kStrFormatWidth = 40;

	std::printf("-- Summary --\n");

	std::printf("Spend                    %*.8f     THB\n", kStrFormatWidth, amountToSpend);
	std::printf(" |_ Buy CRYPTO at price  %*.8f     THB\n", kStrFormatWidth, CRYPTO_buyPrice);
	std::printf(" |_ Sell CRYPTO at price %*.8f     THB\n\n", kStrFormatWidth, CRYPTO_sellPrice);

	if (isIncludeGrossCompute)
	{
		if (grossProfit < 0.0 && g_supportTTY)
			std::printf("Gross profit:            %s%*.8f%s     THB\n", kPrefixRedTxt, kStrFormatWidth, grossProfit, kPostfixTxt);
		else if (grossProfit > 0.0 && g_supportTTY)
			std::printf("Gross profit:            %s%*.8f%s     THB\n", kPrefixGreenTxt, kStrFormatWidth, grossProfit, kPostfixTxt);
		else
			std::printf("Gross profit:            %*.8f     THB\n", kStrFormatWidth, grossProfit);
	}

	{
		if (profit < 0.0 && g_supportTTY)
			std::printf("Profit:                  %s%*.8f%s     THB\n", kPrefixRedTxt, kStrFormatWidth, profit, kPostfixTxt);
		else if (profit > 0.0 && g_supportTTY)
			std::printf("Profit:                  %s%*.8f%s     THB\n", kPrefixGreenTxt, kStrFormatWidth, profit, kPostfixTxt);
		else
			std::printf("Profit:                  %*.8f     THB\n", kStrFormatWidth, profit);
	}
	
	std::printf("Total fee:               %*.8f     THB\n", kStrFormatWidth, buyFee + sellFee);
	std::printf(" |_ Buy fee:             %*.8f     THB\n", kStrFormatWidth, buyFee);
	std::printf(" |_ Sell fee:            %*.8f     THB\n", kStrFormatWidth, sellFee);
	if (isIncludeGrossCompute)
		std::printf("Gross CRYPTO amount:     %*.8f     CRYPTO\n", kStrFormatWidth, grossAmountGainedCRYPTO);
	std::printf("CRYPTO amount:           %*.8f     CRYPTO\n\n", kStrFormatWidth, netAmountGainedCRYPTO);

	if (isIncludeProfitReferencePoints)
	{
		// compute further more margin of sell price for 0.1%, 0.15%, 0.2%, 0.25%, 0.5%, 1%, 1.5%, 2%, and 5%
		std::printf("-- Profit Reference Points --\n");
		ComputeAndPrintProfitReferencePointEntry(0.1, CRYPTO_sellPrice, kStrFormatWidth, grossSellPrice, netSellPrice, kFeePercentage, amountToSpend, isIncludeGrossCompute);
		ComputeAndPrintProfitReferencePointEntry(0.15, CRYPTO_sellPrice, kStrFormatWidth, grossSellPrice, netSellPrice, kFeePercentage, amountToSpend, isIncludeGrossCompute);
		ComputeAndPrintProfitReferencePointEntry(0.2, CRYPTO_sellPrice, kStrFormatWidth, grossSellPrice, netSellPrice, kFeePercentage, amountToSpend, isIncludeGrossCompute);
		ComputeAndPrintProfitReferencePointEntry(0.25, CRYPTO_sellPrice, kStrFormatWidth, grossSellPrice, netSellPrice, kFeePercentage, amountToSpend, isIncludeGrossCompute);
		ComputeAndPrintProfitReferencePointEntry(0.5, CRYPTO_sellPrice, kStrFormatWidth, grossSellPrice, netSellPrice, kFeePercentage, amountToSpend, isIncludeGrossCompute);
		ComputeAndPrintProfitReferencePointEntry(1.0, CRYPTO_sellPrice, kStrFormatWidth, grossSellPrice, netSellPrice, kFeePercentage, amountToSpend, isIncludeGrossCompute);
		ComputeAndPrintProfitReferencePointEntry(1.5, CRYPTO_sellPrice, kStrFormatWidth, grossSellPrice, netSellPrice, kFeePercentage, amountToSpend, isIncludeGrossCompute);
		ComputeAndPrintProfitReferencePointEntry(2.0, CRYPTO_sellPrice, kStrFormatWidth, grossSellPrice, netSellPrice, kFeePercentage, amountToSpend, isIncludeGrossCompute);
		ComputeAndPrintProfitReferencePointEntry(2.5, CRYPTO_sellPrice, kStrFormatWidth, grossSellPrice, netSellPrice, kFeePercentage, amountToSpend, isIncludeGrossCompute);
		ComputeAndPrintProfitReferencePointEntry(5.0, CRYPTO_sellPrice, kStrFormatWidth, grossSellPrice, netSellPrice, kFeePercentage, amountToSpend, isIncludeGrossCompute);
	}
	return 0;
}
