


/*
 * 1. 钱能保留出来投资是最好的，即使不能完全抵消产生的利息，也会让总利息减少
 * 2. 等额本息有两个好处：前期月供低，可以剩余钱来进行投资产生收益
 * 3. 所得利息收入要尽快继续投资，由于等额本息省下的本金也要尽快投入
 */


#include <stdio.h>
#include <math.h>


//----------------------------------------------

// 前期总预算，包含装修
#define TOTAL_BUDGET ((186 - 85.29) * 10000)
#define DECORATE_BUDGET (5 * 10000)  //装修费

#define TOTAL_PRICE (274.71 * 10000) // 总房价
#define SQUARE (91.57) // m2
#define REF_PRICE_PER_M2 (16247.66) // 指导价

#define PUBLIC_FEE_PRICE_PER_M2 (200) // 公共维修基金每平米价格

// 固定常数
#define FIRST_INITIAL_RATE (0.8)
#define SECOND_INITIAL_RATE (0.7)
#define MAX_LOAN_MONTH (360) // 贷款年限（月）
#define ESTIMATE_RATE (1.0) // 评估比例
#define EXTRA_RATE (0.0162) // 中介费比例
#define BUSINESS_LOAN_RATE (0.054 * 0.9) //商贷比例

#define HOUSE_FUND_NUM (120 * 10000) // 公积金贷款额
#define HOUSE_FUND_RATE (0.035) //公积金贷款比例

#define INVEST_RATE (0.08) // 投资年化收益率

//----------------------------------------------

float total_price = TOTAL_PRICE;
float min_public_price = SQUARE * REF_PRICE_PER_M2; // 指导价
float estimate_price; // 评估价 == 网签价
float registered_price;
float max_loan; // 可贷款总额
float business_loan;// 商贷额度
float initial_price; // 首付
float initial_price_extra; // 单独给业主的首付
float extra_pay; // 中介费
float contract_tax;
float business_tax;
float personal_tax;
float public_maintain; // 公共维修基金
float remain_management_fee; // 海清园物业管理费 
int max_loan_month;
float remain_num;

//----------------------------------------------

void print_info (void)
{
	if (SQUARE <= 90.0)
		contract_tax = registered_price * 0.01;
	else if ((SQUARE > 90.0) && (SQUARE <= 140.0))
		contract_tax = registered_price * 0.015;
	else 
		contract_tax = registered_price * 0.03;	

	
	printf("=======================================\n");
	printf("成交价        ：%f\n", total_price);
	printf("最低网签价    ：%f\n", min_public_price);
	printf("实际网签价    ：%f\n", registered_price);
	printf("契税          ：%f\n", contract_tax);
	printf("营业税        ：%f\n", business_tax);
	printf("个税          ：%f\n", personal_tax);
	printf("最高贷款额    ：%f\n", max_loan);
	printf("公积金贷款    ：%d\n", HOUSE_FUND_NUM);
	printf("商业贷款      ：%f\n", business_loan);
	printf("首付          ：%f\n", initial_price);
	printf("首付业主      ：%f\n", initial_price_extra);
	printf("公共维修基金  ：%f\n", public_maintain);
	printf("中介费        ：%f\n", extra_pay);
	printf("海清园剩余费  ：%f\n", remain_management_fee);
	printf("总首付        ：%f\n", initial_price_extra + initial_price + 
                  extra_pay + contract_tax + remain_management_fee + public_maintain);
	printf("预留装修款    ：%d\n", DECORATE_BUDGET);
	printf("剩余	      ：%f\n", remain_num);
	printf("=======================================\n"); 
} 

// -------------------------------------------------
void initial_info (void)
{
	estimate_price =  total_price * ESTIMATE_RATE; // 评估价 == 网签价
	registered_price = estimate_price;

	max_loan = (estimate_price * 0.7); // 可贷款总额
	initial_price = total_price - max_loan; // 首付
	initial_price_extra = 85.29 * 10000; // 首付
	extra_pay = 360 * 10000 * EXTRA_RATE; // 中介费

	contract_tax = 0;
	business_tax = 0;
	personal_tax = 0;
	public_maintain = SQUARE * PUBLIC_FEE_PRICE_PER_M2;
	remain_management_fee = 8000;

	max_loan_month = MAX_LOAN_MONTH;
	business_loan = max_loan - HOUSE_FUND_NUM;
	remain_num = TOTAL_BUDGET - initial_price - contract_tax
		- extra_pay - business_tax - personal_tax 
		- DECORATE_BUDGET - remain_management_fee
                - public_maintain;
}

// -------------------------------------------------

int main (int argc, char *argv[])
{
	// Business loan	
	int loan_num; 
	float rate = BUSINESS_LOAN_RATE / 12; // 5.4% / 12, month rate
	int i = 0;
	float fee = 0;
	float fact = 0;
	float total_fee = 0;
	float total_bj = 0;
	float bj = 0;

	float bx_fee = 0;
	float bx_loan_num = 0;
	float bx_total_fee = 0;
	float bx_monthly = 0;

	// House Fund loan	
	int house_fund_num = HOUSE_FUND_NUM; // 120w
	float house_fund_rate = HOUSE_FUND_RATE / 12; // month rate
	float house_fund_fee = 0;
	float house_fund_fact = 0;
	float house_fund_total_bj = 0;
	float house_fund_bj = 0;

	int bx_house_fund_num = HOUSE_FUND_NUM; // 120w
	float bx_house_fund_fee = 0;
	float bx_house_fund_monthly = 0;

	// savings = bj - bx
	float savings = 0;
	float total_savings = 0;
	float bx_benefit = 0;
	float tmp = 0;
	float max_bx_benefit = 0;

	// Investment	
	int invest_num = 0;
	float invest_rate = INVEST_RATE / 12;
	float income = 0, total_income = 0, year_income = 0;


	initial_info ();
	print_info ();

	loan_num = bx_loan_num = max_loan;
	fact = loan_num / max_loan_month;
	invest_num = remain_num;
	house_fund_fact = house_fund_num / max_loan_month;
	bx_monthly = (bx_loan_num * rate * pow(1+rate, max_loan_month))
		/ (pow(1+rate, max_loan_month) - 1);

	printf("Business Loan, monthly pay = %d, bx monthly pay = %f\n", fact, bx_monthly);
	for (i = 0; i < max_loan_month; i++) {
		fee = loan_num * rate;
		loan_num -= fact; 
		total_fee += fee;

		bx_fee = bx_loan_num * rate;
		bx_loan_num -= (bx_monthly - bx_fee);
		if (bx_loan_num < 0)
			bx_loan_num = 0;
		bx_total_fee += bx_fee;

		if (!(i % 12))
			printf("\n\n---------------- YEAR %d --------------\n",
					 i/12 + 1);
		printf("Month %03d, fee %05f, monthly %05f, bx_load_num = %f, bx_fee %05f, bx_monthly %05f\n", 
			i+1, fee, fee + fact, bx_loan_num, bx_fee, bx_monthly);

		if ((i %12) == 11)
			printf("Total fee = %f, bx total fee = %f\n", total_fee, bx_total_fee);
	}


	printf("\n\nHouse Fund + Business Loan\n");	
	total_fee = bx_total_fee = 0;
	bx_loan_num = loan_num = business_loan; 
	bx_monthly = (bx_loan_num * rate * pow(1+rate, max_loan_month))
		/ (pow(1+rate, max_loan_month) - 1);
	bx_house_fund_monthly = (bx_house_fund_num * house_fund_rate * pow(1+house_fund_rate, max_loan_month))
		/ (pow(1+house_fund_rate, max_loan_month) - 1);
	fact = loan_num / max_loan_month;
	for (i = 0; i < max_loan_month; i++) {
		house_fund_fee = house_fund_num * house_fund_rate;
		house_fund_num -= house_fund_fact; 
		total_fee += house_fund_fee;

		bx_house_fund_fee = bx_house_fund_num * house_fund_rate;
		house_fund_bj = (bx_house_fund_monthly - bx_house_fund_fee);
		house_fund_total_bj += house_fund_bj;
		bx_house_fund_num -= house_fund_bj;
		bx_total_fee += bx_house_fund_fee;

		fee = loan_num * rate;
		loan_num -= fact;
		total_fee += fee;	

		bx_fee = bx_loan_num * rate;
		bj = (bx_monthly - fee);
		bx_loan_num -= bj;
		total_bj += bj;
		bx_total_fee += bx_fee;

		savings += fee + house_fund_fee + house_fund_fact + fact - bx_monthly - bx_house_fund_monthly;
		if (savings <= 0)
			savings = 0;

		if (!(i % 12))
			printf("\n\n---------------- YEAR %d --------------\n", 
				i/12 + 1);

		printf("Month %03d, house_fund_fee %05f, fee %05f, monthly %05f, bx_house_fund_fee %f, bx_fee %f, bx_monthly %f\n", \
			i+1, house_fund_fee, fee, fee + house_fund_fee + house_fund_fact + fact,\
			bx_house_fund_fee, house_fund_fee, bx_monthly + bx_house_fund_monthly);

		if ((i % 12) == 11) {
			total_savings += savings;
			bx_benefit = total_savings - (bx_total_fee - total_fee);
			if (bx_benefit > max_bx_benefit)
				max_bx_benefit = bx_benefit;
			printf("Total fee = %f, bx total fee %f, savings in this year %f, total_savings %f\n", total_fee, bx_total_fee, savings, total_savings);
			printf("bj spend house_fund benjin = %f, bj spend benjin %f, tatal %f\n", (i+1)*house_fund_fact, (i+1)*fact, 
					(i+1)*(house_fund_fact+fact));
			printf("bx spend house_fund benjin = %f, bx spend benjin %f, total %f\n", house_fund_total_bj, total_bj,
					house_fund_total_bj+total_bj);
			printf("bx_benefit = %f, max_bx_benefit %f\n", bx_benefit, max_bx_benefit);
			savings = 0;
		}
	}


	printf("\n\nInvest income %d\n", invest_num);
	for (i = 0; i < max_loan_month; i++) {
		income = invest_num * invest_rate;
		year_income += income;

		if (!(i % 12))
			printf("\n\n---------------- YEAR %d --------------\n", i/12 + 1);
		printf("Month %03d, income %05f\n", i+1, income);

		if ((i %12) == 11) {
			printf("year income = %f\n", year_income);
			total_income += year_income;
			printf("Total income = %f\n", total_income);
			invest_num += year_income;
			year_income = 0;
		}
	}

	return 0;
}
