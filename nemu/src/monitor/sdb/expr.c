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
#include <vector>
enum {
  TK_NOTYPE = 256, TK_EQ,
	NUM_TYPE = '0',
  /* TODO: Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
	{"\\(", '('},
	{"\\)", ')'},
	{"\\*", '*'},
	{"/", '/'},
  {"\\+", '+'},         // plus
	{"-", '-'},
  {"==", TK_EQ},        // equal
	{"[0-9]*", NUM_TYPE},
};

#define NR_REGEX ARRLEN(rules)
map <int, int> priority[NR_REGEX + 1];
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
} Token;

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
					printf("invalid input number!\n");
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
							tokens[nr_token].type = rules[i].token_type;
							strncpy(tokens[nr_token].str, substr_start, substr_len);
							nr_token += 1;
							break;
					default:
							printf("no store\n");
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
	if(tokens[sta].type != '(' || tokens[end].type != ')')
		return false;//need both sides are parentheses
	//if meet (, then plus 1, else minus 1
	//if not in the end, the res <= 0, false	
	int res = 0;
	for(int i = sta; i <= end; i++){
		int now_type = tokens[i].type;
		switch(now_type):
			case '(':
			  res += 1;
				break;
			case ')':
				res -= 1;
				break;
			default:
				break;
		if(i == end && res == 0)
			return true;
		else if(res == 0)
			return false;
	}
}


word_t find_main_op(int sta, int end){
	//TODO:priority!
	//find the main op
	//pr1: the op doesn't exist between a couple of parentheses,so the left can't be (, the right can't be )
	//pr2: should be operation
	//pr3: lower priority
	//pr4: the same prority, the most right side
	vector<int> ops;//to store the valid operations
	for(int i = sta; i <= end; i++)
	{
		//pr2
		if(tokens[i].type == NUM_TYPE)
			continue;
		//pr1
		int l = i - 1; 
		int r = i + 1;
		bool valid = true;
		while(valid == true && l >= sta)
		{
			if(tokens[l].type == '(')
				valid = false;
			l--;
		}
		while(valid == true && r <= end)
		{
			if(tokens[r].type == ')')
				valid = false;
			r++;
		}
		if(valid == true)
		{
			ops.push_back(i);
		}
	}
	if(ops.size() == 0)
		return -1;//invalid
	//record the priority and location
	pair<int, int> valid_op(-1, 100);//location and priority
	int res = 0;
	for(auto it = ops.begin(); it != ops.end(); it++)
	{
			int type = tokens[*i].type;
			int pri = priority.find(type).second;
			int loc = *it;
			if(pri < valid_op.second || (pri == valid_op.second && loc > valid_op.first))
			{
				valid_op.first = loc;
				valid_op.second = pri;
				res = *it;
			}
	}
	return res;
}

word_t eval(int sta, int end){
	if(sta > end)
		return -1;//bad expressions
	else if(sta == end)
	{
		//single token, just return
		int sin_token;
		sscanf(tokens[sta].str, "%d", &sin_token);
//		printf("sin_token = %d\n", sin_token);
	}
	else if(check_parentheses(sta, end) == true)
	{
		return	eval(sta + 1, end - 1);
	}
	else
	{
		int op = find_main_op(sta, end);
		assert(op != -1);
		char op_type = tokens[op].type;
		switch(op_type):
			case '+':	return eval(sta, op - 1) + eval(op + 1, end);
			case '-': return eval(sta, op - 1) - eval(op + 1, end);
			case '*': return eval(sta, op - 1) * eval(op + 1, end);
			case '/': return eval(sta, op - 1) / eval(op + 1, end);
			default: assert(0);
	}

}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  /* TODO: Insert codes to evaluate the expression. */
//  TODO();
	eval(0, nr_token - 1);
//now start to calculate the result
  return 0;
}
