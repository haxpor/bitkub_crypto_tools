/**
 * Cryptocurrency neutral profit calculator.
 * Based on bitkub's market.
 */
#include <iostream>
#include <stdlib.h>
#include <cstring>

// fee percentage (already applied percentage)
static constexpr double kFeePercentage = 0.0025;

// accept the target buy price, and target sell price later
// to compute the profit
int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		std::cerr << "Usage: pcalc <amount-to-spend> <CRYPTO-buy-price> <CRYPTO-sell-price>\n";
		return -1;
	}	

	if (std::strcmp(argv[0], "--help") == 0)
	{
		std::cout << "Usage: pcalc <amount-to-spend> <CRYPTO-buy-price> <CRYPTO-sell-price>\n";
		return 0;
	}

	double amountToSpend = atof(argv[1]);
	double CRYPTO_buyPrice = atof(argv[2]);
	double CRYPTO_sellPrice = atof(argv[3]);

	double buyFee = amountToSpend * kFeePercentage;
	double amountGainedCRYPTO = (amountToSpend - buyFee) / CRYPTO_buyPrice;

	double grossSellPrice = amountGainedCRYPTO * CRYPTO_sellPrice;
	double sellFee = grossSellPrice * kFeePercentage;
	double sellPrice = grossSellPrice - sellFee;

	double profit = sellPrice - amountToSpend;

	std::printf("* Summary *\n");
	std::printf("Spend                                      %*.8f     THB\n", 30, amountToSpend);
	std::printf(" |_ Buy CRYPTO at price                    %*.8f     THB\n", 30, CRYPTO_buyPrice);
	std::printf(" |_ Sell CRYPTO at price                   %*.8f     THB\n\n", 30, CRYPTO_sellPrice);

	std::printf("Profit:                                    %*.8f     THB\n", 30, profit);
	std::printf("Total fee:                                 %*.8f     THB\n", 30, buyFee + sellFee);
	std::printf(" |_ Buy fee:                               %*.8f     THB\n", 30, buyFee);
	std::printf(" |_ Sell fee:                              %*.8f     THB\n", 30, sellFee);
	std::printf("CRYPTO amount:                             %*.8f     CRYPTO\n\n", 30, amountGainedCRYPTO);

	// compute further more margin of sell price for 0.1%, 0.15%, 0.2%, 0.25%, 0.5%, 1%, 1.5%, 2%, and 5%
	std::printf("* Profit Reference Point *\n");
	std::printf("Sell at 0.10%% more at  %.8f THB:          %*.8f     THB\n", CRYPTO_sellPrice*1.001, 20, (grossSellPrice*1.001)-(grossSellPrice*1.001*kFeePercentage)-amountToSpend);
	std::printf("Sell at 0.15%% more at  %.8f THB:          %*.8f     THB\n", CRYPTO_sellPrice*1.0015, 20, (grossSellPrice*1.0015)-(grossSellPrice*1.0015*kFeePercentage)-amountToSpend);
	std::printf("Sell at 0.20%% more at  %.8f THB:          %*.8f     THB\n", CRYPTO_sellPrice*1.002, 20, (grossSellPrice*1.002)-(grossSellPrice*1.002*kFeePercentage)-amountToSpend);
	std::printf("Sell at 0.25%% more at  %.8f THB:          %*.8f     THB\n", CRYPTO_sellPrice*1.0025, 20, (grossSellPrice*1.0025)-(grossSellPrice*1.0025*kFeePercentage)-amountToSpend);
	std::printf("Sell at 0.50%% more at  %.8f THB:          %*.8f     THB\n", CRYPTO_sellPrice*1.005, 20, (grossSellPrice*1.005)-(grossSellPrice*1.005*kFeePercentage)-amountToSpend);
	std::printf("Sell at 1.00%% more at  %.8f THB:          %*.8f     THB\n", CRYPTO_sellPrice*1.01, 20, (grossSellPrice*1.01)-(grossSellPrice*1.01*kFeePercentage)-amountToSpend);
	std::printf("Sell at 1.50%% more at  %.8f THB:          %*.8f     THB\n", CRYPTO_sellPrice*1.015, 20, (grossSellPrice*1.015)-(grossSellPrice*1.015*kFeePercentage)-amountToSpend);
	std::printf("Sell at 2.00%% more at  %.8f THB:          %*.8f     THB\n", CRYPTO_sellPrice*1.020, 20, (grossSellPrice*1.020)-(grossSellPrice*1.020*kFeePercentage)-amountToSpend);
	std::printf("Sell at 5.00%% more at  %.8f THB:          %*.8f     THB\n", CRYPTO_sellPrice*1.050, 20, (grossSellPrice*1.050)-(grossSellPrice*1.050*kFeePercentage)-amountToSpend);
	return 0;
}
