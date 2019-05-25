// ================【5种方式输出补码和原码】======================
// @ author         :       zhyh2010
// @ date           :       20150530
// @ version        :       1.1
// @ description    :       5 种方式 实现补码 和 原码的输出
//

#define DateType long
#define BitNum 32

void findNegCode_iter(DateType num, char * code, int bitNum, int cur_iter_num)
{
    if (cur_iter_num == bitNum)
        return;

    code[cur_iter_num] = (num & 1 << (BitNum - 1 - cur_iter_num)) == 0 ? '0' : '1';
    findNegCode_iter(num, code, bitNum, cur_iter_num + 1);
}

void findNegCode(DateType num, char * code, int bitNum)
{
    DateType inum = num;
//  =============【for】============
//  for (int i = 0; i != bitNum; i++)
//      code[i] = (inum & 1 << (BitNum - 1 - i)) == 0 ? '0' : '1';

//  =============【while】============
//  int i = 0;
//  while (i != bitNum)
//      code[i++] = (inum & 1 << (BitNum - 1 - i)) == 0 ? '0' : '1';

//  =============【do while】============
//  int i = 0;
//  do 
//  {
//      code[i++] = (inum & 1 << (BitNum - 1 - i)) == 0 ? '0' : '1';
//  } while (i != bitNum);

//  =============【goto】============
//  int i = 0;
// LOOP:
//  code[i++] = (inum & 1 << (BitNum - 1 - i)) == 0 ? '0' : '1';
//  if (i != bitNum)
//      goto LOOP;

    //  =============【recursive】============
    findNegCode_iter(num, code, bitNum, 0);
}

void findOriCode(DateType num, char * code, int bitNum)
{
    findNegCode(num, code, bitNum);

    if (num > 0)
        return;

    findNegCode(num * (-1), code, bitNum);
    code[0] = '1';
}

void main()
{
    for (DateType num = -5; num != 6; num++)
    {
        char OriCode[BitNum + 1] = { 0 };
        char NegCode[BitNum + 1] = { 0 };

        findOriCode(num, OriCode, BitNum);
        findNegCode(num, NegCode, BitNum);

        printf("num = %d\n\t原码:\t%s\n\t补码:\t%s\n", num, OriCode, NegCode);
    }

    return;
}
--------------------- 
作者：枯萎的海风 
来源：CSDN 
原文：https://blog.csdn.net/zhyh1435589631/article/details/46277769 
版权声明：本文为博主原创文章，转载请附上博文链接！