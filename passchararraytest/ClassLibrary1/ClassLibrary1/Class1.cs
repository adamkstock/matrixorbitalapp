using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ManagedDLL
{
    //interface declaration
    public interface ICalculator
    {
        int Add(int Number1, int Number2);
        char[,] Passback();
    };

    //interface implementation 
    public class ManagedClass : ICalculator
    {
        public int Add(int Number1, int Number2)
        {
            return Number1 + Number2;
        }
        public char[,] Passback ()
        {
            char[,] passarray = new char[65, 20];
            passarray[1, 1] = 'x';
            passarray[2, 1] = 'x';
            passarray[3, 1] = 'x';
            passarray[4, 1] = 'x';
            passarray[1, 3] = 'x';
            passarray[2, 3] = 'x';
            passarray[3, 3] = 'x';
            passarray[4, 3] = 'x';

            passarray[1, 8] = 't';
            passarray[2, 8] = 'h';
            passarray[3, 8] = 'i';
            passarray[4, 8] = 's';
            passarray[5, 8] = ' ';
            passarray[6, 8] = 'i';
            passarray[7, 8] = 's';
            passarray[8, 8] = ' ';
            passarray[9, 8] = 'a';


            passarray[1, 9] = 't';
            passarray[2, 9] = 'e';
            passarray[3, 9] = 's';
            passarray[4, 9] = 't';
            passarray[5, 9] = ' ';
            passarray[6, 9] = 'o';
            passarray[7, 9] = 'f';
            passarray[8, 9] = ' ';
            passarray[9, 9] = 't';
            passarray[10, 9] = 'h';
            passarray[11, 9] = 'e';
            passarray[12, 9] = ' ';

            passarray[1, 10] = 'a';
            passarray[2, 10] = 'r';
            passarray[3, 10] = 'r';
            passarray[4, 10] = 'a';
            passarray[5, 10] = 'y';




            return passarray;
        }
    }
}
