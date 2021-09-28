#include <variant>
#include <iostream>


struct ExpNode_VarRef {

};

struct ExpNode_Integer {

};

struct ExpNode_UnaryOp {

};

struct ExpNode_BinaryOp {

};

using ExpNode = std::variant<
	ExpNode_VarRef,
	ExpNode_Integer,
	ExpNode_UnaryOp,
	ExpNode_BinaryOp
>;


class Analyzer {
private:
	void ReadExpNode(const ExpNode_VarRef& exp_node_var_ref) {
		std::cout << "var ref" << std::endl;
	}
	void ReadExpNode(const ExpNode_Integer& exp_node_integer) {
		std::cout << "integer" << std::endl;
	}
	void ReadExpNode(const ExpNode_UnaryOp& exp_node_unary_op) {
		std::cout << "unary op" << std::endl;
	}
	void ReadExpNode(const ExpNode_BinaryOp& exp_node_binary_op) {
		std::cout << "binary op" << std::endl;
	}

public:
	void ReadExpNodeBase(const ExpNode& exp_node_base) {
		std::visit([&](auto&& exp_node) { ReadExpNode(exp_node); }, exp_node_base);
	}
};


ExpNode GetExpNode() {
	while (true) {
		int i; std::cin >> i;
		switch (i) {
		case 0: return ExpNode_VarRef{};
		case 1: return ExpNode_Integer{};
		case 2: return ExpNode_UnaryOp{};
		case 3: return ExpNode_BinaryOp{};
		}
	}
}


int main() {
	Analyzer analyzer;
	ExpNode exp_node_base = GetExpNode();
	analyzer.ReadExpNodeBase(exp_node_base);
}


/*
00007FF6270C1000  sub         rsp,38h  
00007FF6270C1004  mov         rax,qword ptr [__security_cookie (07FF6270C5008h)]  
00007FF6270C100B  xor         rax,rsp  
00007FF6270C100E  mov         qword ptr [rsp+28h],rax  
00007FF6270C1013  mov         rcx,qword ptr [__imp_std::cin (07FF6270C30D0h)]  
00007FF6270C101A  lea         rdx,[rsp+20h]  
00007FF6270C101F  call        qword ptr [__imp_std::basic_istream<char,std::char_traits<char> >::operator>> (07FF6270C3080h)]  

00007FF6270C1025  mov         ecx,dword ptr [rsp+20h]  
00007FF6270C1029  test        ecx,ecx  
00007FF6270C102B  je          main+57h (07FF6270C1057h)  
00007FF6270C102D  sub         ecx,1  
00007FF6270C1030  je          main+4Eh (07FF6270C104Eh)  
00007FF6270C1032  sub         ecx,1  
00007FF6270C1035  je          main+45h (07FF6270C1045h)  
00007FF6270C1037  cmp         ecx,1  
00007FF6270C103A  jne         main+13h (07FF6270C1013h)  

00007FF6270C103C  lea         rdx,[string "binary op" (07FF6270C32D0h)]  
00007FF6270C1043  jmp         main+5Eh (07FF6270C105Eh)  
00007FF6270C1045  lea         rdx,[string "unary op" (07FF6270C32C0h)]  
00007FF6270C104C  jmp         main+5Eh (07FF6270C105Eh)  
00007FF6270C104E  lea         rdx,[string "integer" (07FF6270C32B8h)]  
00007FF6270C1055  jmp         main+5Eh (07FF6270C105Eh)  
00007FF6270C1057  lea         rdx,[string "var ref" (07FF6270C32B0h)]  

00007FF6270C105E  mov         rcx,qword ptr [__imp_std::cout (07FF6270C30D8h)]  
00007FF6270C1065  call        std::operator<<<std::char_traits<char> > (07FF6270C1090h)  
00007FF6270C106A  lea         rdx,[std::endl<char,std::char_traits<char> > (07FF6270C1260h)]  
00007FF6270C1071  mov         rcx,rax  
00007FF6270C1074  call        qword ptr [__imp_std::basic_ostream<char,std::char_traits<char> >::operator<< (07FF6270C3088h)]  
00007FF6270C107A  xor         eax,eax  
00007FF6270C107C  mov         rcx,qword ptr [rsp+28h]  
00007FF6270C1081  xor         rcx,rsp  
00007FF6270C1084  call        __security_check_cookie (07FF6270C1320h)  
00007FF6270C1089  add         rsp,38h  
00007FF6270C108D  ret 
*/