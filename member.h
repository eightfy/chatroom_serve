#pragma once
#pragma once
#ifndef MEMBER_H
#define MEMBER_H

template<class T>
class bag
{
public:
	bag(T* n, T* m);
	~bag();
	void send();
private:
	//int num;
	//int flag;
	T name[100];
	time_t t;
	T message[1000];
};

#endif // !1
