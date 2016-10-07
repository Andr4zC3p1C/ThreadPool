#include "ThreadPool.h"
#include <time.h>
#include <stdlib.h>

std::mutex tabMutex;

/*
    Simple bubble sort implementation
*/
class Sort : public ThreadTask
{
public:
    Sort(unsigned int size = 20) :
        m_size(size)
    {
        m_tab = new int[m_size];

        for(unsigned int i=0; i < m_size; i++)
        {
            m_tab[i] = rand() % 100;
        }
    }

    ~Sort()
    {
        delete [] m_tab;
    }

    virtual void run()
    {
        bool swapped = false;
        unsigned int l = m_size;
        int tmp;

        do
        {
            swapped = false;

            for(unsigned int i = 0; i < l - 1; i++)
            {
                if(m_tab[i] > m_tab[i+1])
                {
                    tmp = m_tab[i];
                    m_tab[i] = m_tab[i+1];
                    m_tab[i+1] = tmp;

                    swapped = true;
                }
            }

            l--;
        } while(swapped);

        printArray();
        tp_printf("Done!\n\n");
    }

private:
    unsigned int m_size;
    int* m_tab;

    void printArray()
    {
        std::unique_lock<std::mutex> tabLock(tabMutex);

        if(m_size > 20)
            return;

        tp_printf("%d", m_tab[0]);

        for(unsigned int i = 1; i < m_size; i++)
        {
            tp_printf(", %d", m_tab[i]);
        }

        tp_printf("\n");
    }
};

int main()
{
    srand(time(0));

    ThreadPool tp;

    tp_printf("Created %d threads\n\n", tp.getNumThreads());

    // pushing 10 tasks to be executed on threads
    for(unsigned int i=0; i < 10; i++)
    {
        tp.pushTask(new Sort());
    }
}
