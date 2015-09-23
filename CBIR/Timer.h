/******************************************************************
�ļ�����		Timer.h
�����ˣ�		zhaoweiwang
��ע���ڣ�	2013-4-5
������		�߾��ȼ�ʱ�����ʵ��
******************************************************************/
#ifndef _TIMER_H_
#define _TIMER_H_

#ifdef _WIN32
typedef __int64 i64 ;
#else
typedef long long i64;
#endif

class CPerfCounter {

public:
    CPerfCounter();
    ~CPerfCounter();
    void Start(void);
    void Stop(void);
    void Reset(void);
    double GetElapsedTime(void);

private:

    i64 _freq;
    i64 _clocks;
    i64 _start;
};

#endif

