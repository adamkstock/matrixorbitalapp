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
    };

    //interface implementation 
    public class ManagedClass : ICalculator
    {
        public int Add(int Number1, int Number2)
        {
            return Number1 + Number2;
        }
    }
}
