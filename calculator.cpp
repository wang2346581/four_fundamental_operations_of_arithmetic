#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

string get_num(const string& expr, string::iterator& iter){
    string tmp = "";
    auto num_dot = 0;
    do{
        tmp += *iter++;
        if(*iter == '.'){
            tmp += *iter++;
            ++num_dot;
        }
        if(num_dot > 1)
            break;
    } while( isdigit(*iter) && iter != expr.end() );
    return tmp;
}

int get_priority(const string& s){
    if(s == "(" || s == ")")
        return 0;
    else if (s == "+" || s == "-")
        return 1;
    else if (s == "*" || s == "/")
        return 2;
    return 0;
}

int main()
{
    string expr = " -4.2 + 2*8+(10*4+2)* (-13.5 +2.5) * 4 * 2 *(-3.5+ 5.5) /2";
    // step1: Erase redundant space
    expr.erase(
        remove_if(expr.begin(), expr.end(), [](char &c){ return isspace(c); }),
        expr.end()    
    );
    cout << "[step1] Erase redundant space: " << endl;
    cout << expr << endl;

    // step2: split string as nodes
    vector<string> svec;
    auto iter = expr.begin();
    while(iter != expr.end()){
        string tmp = "";
        if(isdigit(*iter)){
            tmp = get_num(expr, iter);
            svec.push_back(tmp);
        }
        else if(*iter == '-'){
            auto iter_prev = iter - 1;
            if(iter == expr.begin() || *iter_prev != ')' || *iter_prev == '+'){ // negative num
                tmp = get_num(expr, iter);
                svec.push_back(tmp);
            }
            else{
                tmp = *iter;
                svec.push_back(tmp);
                ++iter;
            }
        }
        else{
            tmp = *iter;
            svec.push_back(tmp);
            ++iter;
        }
    }
    // print vector of all nums and ops
    cout << "[step2] split string as nodes: " << endl;
    for(auto s: svec)
        cout << s << " ";
    cout << endl;

    // step3: convert to postfix expression (using priority to push_back into vector)
    vector <string> svec2;
    vector <string> vec_ops;
    for(auto s: svec){
        if(s == "+" || s == "-" || s == "*" || s == "/"){
            auto cur_priority = get_priority(s);
            if(vec_ops.size() > 0) {
                auto cur_top = vec_ops.back();
                while(cur_priority <= get_priority(cur_top)){
                    svec2.push_back(cur_top);
                    vec_ops.pop_back();
                    cur_top = vec_ops.back();
                }
            }
            vec_ops.push_back(s);
        }
        else if(s == "("){
            vec_ops.push_back(s);
        }
        else if(s == ")"){ // when it's ")", pop_back untill occuring "("
            auto cur_top = vec_ops.back();
            while(cur_top != "("){
                svec2.push_back(cur_top);
                vec_ops.pop_back();
                cur_top = vec_ops.back();
            }
            vec_ops.pop_back(); // pop out "("
        }
        else{
            svec2.push_back(s);
        }
    }
    while(vec_ops.size() > 0){ // pop remain ops
        auto cur_top = vec_ops.back();
        svec2.push_back(cur_top);
        vec_ops.pop_back();
    }

    cout << "[step3] postFix: " << endl;
    for(auto s: svec2)
        cout << s << " ";
    cout << endl;

    // step4: calculate
    string total_string = "";
    vector <string> vec_nums;
    // traverse the vector, and calculate the ans
    for(auto s: svec2){
        if(s == "+" || s == "-" || s == "*" || s == "/"){
            auto val2 = vec_nums.back();
            vec_nums.pop_back();
            auto val1 = vec_nums.back();
            vec_nums.pop_back();
            if(s == "+")
                total_string = to_string(stod(val1) + stod(val2));
            else if(s == "-")
                total_string = to_string(stod(val1) - stod(val2));
            else if(s == "*")
                total_string = to_string(stod(val1) * stod(val2));
            else if(s == "/")
                total_string = to_string(stod(val1) / stod(val2));
            vec_nums.push_back(total_string);
        }
        else{
            vec_nums.push_back(s);
        }
    }
    auto ans = stod(vec_nums.back());
    cout << "[step4] The answer is: " << ans;
    return 0;
}