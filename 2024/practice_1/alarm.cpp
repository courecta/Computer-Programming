#include <stdio.h>
// #include <vector>

int main(){
    int h_1(0), h_2(0), m_1(0), m_2(0);
    int total_minutes(0);
    // vector<int> answers;

    while(1)
    {
        total_minutes = 0;
        scanf("%d%d%d%d", &h_1, &m_1, &h_2, &m_2);
        if(h_1 == 0 && m_1 == 0 && h_2 == 0 && m_2 == 0) break;

        if(h_1 == h_2 && m_2 >= m_1)
        {
            if(m_2 == m_1)
            {
                printf("0\n");
            }
            else printf("%d\n", m_2 - m_1);

            continue;
        }
        
        while(1)
        {
            total_minutes += 60;
            h_1++;
            if(h_1 == 24) h_1 = 0;

            if(m_1 < m_2)
            {
                total_minutes += m_2 - m_1;
                m_1 = m_2;
            }

            if(h_1 == h_2 && m_1 > m_2)
            {
                total_minutes -= m_1 - m_2;
                m_1 = m_2;
            }
            if(h_1 == h_2 && m_1 == m_2) break;
        }
        
        printf("%d\n", total_minutes);

    }

}