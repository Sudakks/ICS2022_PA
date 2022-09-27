/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
enum {
  TK_NOTYPE = 256, TK_EQ,
	NUM_TYPE = '0',
  /* TODO: Add more token types */

};
bool valid_expr = true;//stands whether the expression is valid

static struct rule {
  const char *regex;
  int token_type;
	int pri;//priority
} rules[] = {
  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
	{"\\(", '(', 0},
	{"\\)", ')', 0},
	{"\\*", '*', 2},
	{"/", '/', 2},
  {"\\+", '+', 1},         // plus
	{"-", '-', 1},
  {"==", TK_EQ, 0},        // equal
	{"[0-9]*", NUM_TYPE, 0},
};

#define NR_REGEX ARRLEN(rules)
static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
	int pri;//priority
} Token;
//need to clear the whole arrays

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;
  nr_token = 0;
    /* Try all rules one by one. */
		while(e[position] != '\0'){
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

      Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;
        if(substr_len > 32 && rules[i].token_type == NUM_TYPE)
				{
					printf("Invalid input number! Length is exceeds 32\n");
					return false;
				}//it means the number is greater than the maximum number

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
				//add the recognized token into array
        switch (rules[i].token_type) {
					case TK_NOTYPE:	
							break;
					case NUM_TYPE:
							memset(tokens[nr_token].str, 0, 32);//clear length is 32
							tokens[nr_token].type = rules[i].token_type;
							strncpy(tokens[nr_token].str, substr_start, substr_len);
							nr_token += 1;
							break;
					default:
							if(rules[i].pri > 0)
								tokens[nr_token].pri = rules[i].pri;
							tokens[nr_token].type = rules[i].token_type;
							nr_token += 1;
							break;
//          default: TODO();
        }
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }
  return true;
}

bool check_parentheses(int sta, int end){
	printf("------------\n");
	//to check all parentheses are valid or not
	bool ans_return = true;
	if(tokens[sta].type != '(' || tokens[end].type != ')')
		ans_return = false;//need both sides are parentheses
	//if meet (, then plus 1, else minus 1
	//if not in the end, the res <= 0, false	
	int res = 0;
	for(int i = sta; i <= end && valid_expr == true && ans_return == true; i++){
		int now_type = tokens[i].type;
		if(now_type != '(' || now_type != ')')
			continue;
		printf("reah\n");
		switch(now_type){
			case '(':
			  res += 1;
				if(i == end)
				{
					valid_expr = false;
					printf("HERE B\n");
				}
				else
				{
					int next_type = tokens[i+1].type;
					if(next_type == ')' || next_type == '+' || next_type == '-' || next_type == '*' || next_type == '/')
					{
						valid_expr = false;
						printf("HERE C\n");
					}
					if(i != sta && tokens[i-1].type == NUM_TYPE)
					{
						valid_expr = false;
						printf("HERE D\n");
					}
				}
				/*
					不能是结尾，后面不能跟运算符，前面不能有数字
				*/
				break;
			case ')':
				res -= 1;
				if(i == sta)
				{
					valid_expr = false;
					printf("HERE E\n");
				}
				else
				{
					int last_type = tokens[i-1].type;
					if(last_type == '(' || last_type == '+' || last_type == '-' || last_type == '*' || last_type == '/')
					{
						valid_expr = false;
						printf("HERE F\n");
					}
					if(i != end && (tokens[i+1].type == NUM_TYPE || tokens[i+1].type == '(' || res < 0))
					{
						valid_expr = false;
						 printf("HERE G\n");
					}
				}
				break;
			default:
				break;
		}
		printf("reach here\n");
		printf("now = %d, res = %d\n", i, res);
		if((i != end && res == 0) || valid_expr == false || (i == end && res != 0))
		{
			ans_return = false;
			printf("Yes here, valid_expr == %d\n", valid_expr);
		}
	}
	printf("check: %d, from %d to %d\n", ans_return, sta, end);
	return ans_return;
}


word_t find_main_op(int sta, int end){
	//TODO:priority!
	//find the main op
	//如果检测出来没有合法的表达式，那么就invalid
	//pr1: the op doesn't exist between a couple of parentheses,so the left can't be (, the right can't be )
	//pr2: should be operation
	//pr3: lower priority
	//pr4: the same prority, the most right side
	int ops[NR_REGEX];//to store the valid operations
	int num = 0;
	printf("start from %d to %d, the valid_expr = %d\n", sta, end, valid_expr);
	for(int i = sta; i <= end && valid_expr == true; i++)
	{
		//pr2
		if(tokens[i].pri <= 0)
			continue;
		//judge whether it's valid
		if(tokens[i].type == '+' || tokens[i].type == '-' || tokens[i].type == '*' || tokens[i].type == '/') 
		{
			if(i == sta || i == end || tokens[i-1].type != NUM_TYPE || tokens[i+1].type != NUM_TYPE)
			{
				printf("i = %d, tokens[i-1].type = %d, tokens[i+1].type = %d\n", i, tokens[i-1].type, tokens[i+1].type);
				printf("operator wrong\n");
				valid_expr = false;//如果前后没有操作数，那么此时不合法（但是要考虑到单目运算符）
				break;
			}
		}
		//pr1
		int l = i - 1; 
		int r = i + 1;
		bool valid = true;
		while(valid == true && l >= sta)
		{
			if(tokens[l].type == ')')
				break;
			if(tokens[l].type == '(')
				valid = false;
			l--;
		}
		while(valid == true && r <= end)
		{
			if(tokens[r].type == '(')
				break;
			if(tokens[r].type == ')')
				valid = false;
			r++;
		}
		if(valid == true)
		{
		//	printf("valid in loc %d\n", i);
			ops[num] = i;
			num++;
		}
	}
//	printf("num = %d\n", num);
	if(num == 0)
	{
		printf("wrong from %d to %d\n", sta, end);
		return -1;//invalid
	}
	//record the priority and location
	int res = 0;
	int min_pri = 100;
	int max_loc = -1;
	for(int it = 0; it < num; it++)
	{
			int loc = ops[it];
			int pri = tokens[loc].pri; 
			if(pri < min_pri || (pri == min_pri && loc > max_loc))
			{
				res = loc;
				min_pri = pri;
				max_loc = loc;
			}
	}
//	printf("res = %d\n", res);
//	printf("main op's type = %d\n", tokens[res].type);
	return res;
}

word_t eval(int sta, int end){
	//need to identify the invalid expr
	if(valid_expr == false)
		return 0;
	if(sta > end)
	{
		return 0;//bad expressions
	}
	else if(sta == end)
	{
		//single token, just return
		int sin_token;
		sscanf(tokens[sta].str, "%d", &sin_token);
		return sin_token;
	//printf("sin_token = %d\n", sin_token);
	}
	else if(check_parentheses(sta, end) == true)
	{
//		printf("true in %d, %d\n", sta, end);
		return	eval(sta + 1, end - 1);
	}
	else
	{
		int op = find_main_op(sta, end);
		if(op == -1)
		{
			valid_expr = false;
			printf("HERE A\n");
			return 0;
		}
		int op_type = tokens[op].type;
	//	printf("area1: %d, %d\narea2: %d, %d\n", sta, op-1,op+1,end);
		word_t val1 = eval(sta, op - 1);
		word_t val2 = eval(op + 1, end);
	//	printf("val1 = %u, val2 = %u\n", val1, val2);
	//	printf("type now = %d\n", op_type);
		switch(op_type){
			case '+':	return val1 + val2;
			case '-': return val1 - val2;
			case '*': return val1 * val2;
			case '/': return val1 / val2;
			default: assert(0);
			}
	}
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  /* TODO: Insert codes to evaluate the expression. */
//  TODO();
	*success = true;
//now start to calculate the result
  word_t ans = eval(0, nr_token - 1);
//	printf("valid_expr = %d\n", valid_expr);
	if(valid_expr == true)
	{
		printf("final ans = %d\n", ans);
		return ans;
	}
	else 
	{
		printf("Invalid expression, can't calculate!\n");
		return 0;
	}
}
