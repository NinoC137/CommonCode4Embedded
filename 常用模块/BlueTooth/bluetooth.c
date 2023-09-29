#include "bluetooth.h"
#include "main.h"
#include <string.h>
#include "PID.h"
#include "Motor.h"

unsigned char USART_TX_Buffer[USART_TX_LEN]={0};  //发送数据缓存区
unsigned char USART_RX_Buffer[USART_RX_LEN]={0};  //接受数据缓存区

unsigned char TX_Checksum;
unsigned char cnt_tx,cnt_rx;

unsigned char Frame_Head_sta,Frame_End_sta;

float speed,angle,traget_angle;
extern float Speed_rad0, Speed_M0, Speed_rad1, Speed_M1;
float Speed_OUT, Turn_OUT;

BlueTooth  Transmit;
BlueTooth  Receice;

unsigned short int Blue_mode;
//-------------------------------------------------建议放在while循环里--------------------------------------------+--------------
void Buletooth_Debugger(void)
{
    if(Blue_mode == 0)
    {
        Blue_Receive(&Receice);         //注意：手机端和单片机Receive和Transmit相反

        Blue_mode = Receice.sint_1;
    }

    if(Blue_mode == 1)
    {
//        Transmit.sint_1 = ;
//        Transmit.sint_2 = ;
//        Transmit.sint_3 = ;
//        Transmit.sint_4 = ;
        Transmit.float_1 = speed;
        Transmit.float_2 = (Speed_M0+Speed_M1)/2;
        Transmit.float_3 = angle;
        Transmit.float_4 = traget_angle;
//        Transmit.float_5 = ;
//        Transmit.float_6 = ;
//        Transmit.float_7 = ;
//        Transmit.float_8 = ;
//        Transmit.float_9 = ;
//        Transmit.float_10 = ;
//        Transmit.float_10 = ;
//        Transmit.float_11 = ;

        Blue_Transmit(&Transmit);//注意：手机端和单片机Receive和Transmit相反

        Blue_Receive(&Receice);//注意：手机端和单片机Receive和Transmit相反

        Blue_mode = Receice.sint_1;

        if(Blue_mode == 1)
        {
            Blue_mode = Receice.sint_1;
//             = Receice.sint_2;
//             = Receice.sint_3;
//             = Receice.sint_4;
            speed = Receice.float_1;
            P_Set(&pid_Motor1_Speed, Receice.float_2);
            I_Set(&pid_Motor1_Speed, Receice.float_3);
            D_Set(&pid_Motor1_Speed, Receice.float_4);
            angle = Receice.float_5;
            P_Set(&pid_Turn, Receice.float_6);
            I_Set(&pid_Turn, Receice.float_7);
            D_Set(&pid_Turn, Receice.float_8);
            Speed_OUT = Receice.float_9;
            Turn_OUT = Receice.float_10;
//             = Receice.float_11;
        }
    }
    if(Blue_mode == 2)
    {
//        Transmit.sint_1 = ;
//        Transmit.sint_2 = ;
//        Transmit.sint_3 = ;
//        Transmit.sint_4 = ;
//        Transmit.float_1 = ;
//        Transmit.float_2 = ;
//        Transmit.float_3 = ;
//        Transmit.float_4 = ;
//        Transmit.float_5 = ;
//        Transmit.float_6 = ;
//        Transmit.float_7 = ;
//        Transmit.float_8 = ;
//        Transmit.float_9 = ;
//        Transmit.float_10 = ;
//        Transmit.float_10 = ;
//        Transmit.float_11 = ;

        Blue_Transmit(&Transmit);//注意：手机端和单片机Receive和Transmit相反

        Blue_Receive(&Receice);//注意：手机端和单片机Receive和Transmit相反

        Blue_mode = Receice.sint_1;

        if(Blue_mode == 2)
        {
//				     = Receice.sint_1;
//             = Receice.sint_2;
//             = Receice.sint_3;
//             = Receice.sint_4;
//             = Receice.float_1;
//             = Receice.float_2;
//             = Receice.float_3;
//             = Receice.float_4;
//             = Receice.float_5;
//             = Receice.float_6
//             = Receice.float_7;
//             = Receice.float_8;
//             = Receice.float_9;
//             = Receice.float_10;
//             = Receice.float_11;
        }
    }
}


//-------------------------------------------------------------------------------------------------------------

/* Blue_Transmit的用法
 * 将单片机的数据发送到上位机查看
 * while(1)
 * {
 *      Transmit.sint_1 = xx;(xx为上位机上要显示的sint变量)
 *      Blue_Transmit(&Transmit);
 * }
 * 需要注意的是此处应先将数据传到结构体，再将结构体发送至上位机
 */

void Blue_Transmit(BlueTooth *Tr_inter)
{
    //数据帧头
    USART_TX_Buffer[0] = 0xA5;

    Short_to_Byte(Tr_inter->sint_1,&USART_TX_Buffer[1]);
    Short_to_Byte(Tr_inter->sint_2,&USART_TX_Buffer[3]);
    Short_to_Byte(Tr_inter->sint_3,&USART_TX_Buffer[5]);
    Short_to_Byte(Tr_inter->sint_4,&USART_TX_Buffer[7]);
    Float_to_Byte(Tr_inter->float_1,&USART_TX_Buffer[9]);
    Float_to_Byte(Tr_inter->float_2,&USART_TX_Buffer[13]);
    Float_to_Byte(Tr_inter->float_3,&USART_TX_Buffer[17]);
    Float_to_Byte(Tr_inter->float_4,&USART_TX_Buffer[21]);
    Float_to_Byte(Tr_inter->float_5,&USART_TX_Buffer[25]);
    Float_to_Byte(Tr_inter->float_6,&USART_TX_Buffer[29]);
    Float_to_Byte(Tr_inter->float_7,&USART_TX_Buffer[33]);
    Float_to_Byte(Tr_inter->float_8,&USART_TX_Buffer[37]);
    Float_to_Byte(Tr_inter->float_9,&USART_TX_Buffer[41]);
    Float_to_Byte(Tr_inter->float_10,&USART_TX_Buffer[45]);
    Float_to_Byte(Tr_inter->float_11,&USART_TX_Buffer[49]);

    //计算校验和
    USART_TX_Buffer[USART_TX_LEN-2]=0;//校验位清零
    for(cnt_tx=1;cnt_tx<=USART_TX_LEN-3;cnt_tx++)
        USART_TX_Buffer[USART_TX_LEN-2] += USART_TX_Buffer[cnt_tx];
    USART_TX_Buffer[USART_TX_LEN-2] = USART_TX_Buffer[USART_TX_LEN-2]&0xff;

    //数据帧尾
    USART_TX_Buffer[USART_TX_LEN-1] = 0x5A;

//    UART_transmitBuffer(EUSCI_A2_BASE,(unsigned char*)&USART_TX_Buffer,USART_TX_LEN);
}

/* Blue_Receive的用法
* 用上位机发送数据至单片机
 * while(1)
 * {
 *      Blue_Receive(&Transmit);
 *      xx=Receice.sint_1;(xx为手机上要发送的变量<要改的参数>)
 * }
 * 需要注意的是此处应先用结构体接受上位机传输的数据，再将数据传给指定变量
 */

void Blue_Receive(BlueTooth *Re_inter)
{
    //检查包头，包尾
    if(USART_RX_Buffer[0]==0xA5 && USART_RX_Buffer[USART_RX_LEN-1]==0x5A)
    {
        //检查校验和
        unsigned char err=0;
        for(cnt_rx=1;cnt_rx<=USART_RX_LEN-3;cnt_rx++)
            err += USART_RX_Buffer[cnt_rx];
        err = err&0xff;
        if(err!=USART_RX_Buffer[USART_RX_LEN-2])
        {
            memset( USART_RX_Buffer,0x00,USART_RX_LEN); //将上次接收到的数据清零,重新接收
            Frame_Head_sta=0;   //帧头标志位全部置0
            Frame_End_sta=0;    //帧尾标志位全部置0
        }

            //数据有效  将buff中的字节数据转换成指定类型
        else{//以下几个赋值等号左边可以改动,改变量名
            Re_inter->sint_1  = hex_sint(&USART_RX_Buffer[1]);
            Re_inter->sint_2  = hex_sint(&USART_RX_Buffer[3]);
            Re_inter->sint_3  = hex_sint(&USART_RX_Buffer[5]);
            Re_inter->sint_4  = hex_sint(&USART_RX_Buffer[7]);
            Re_inter->float_1 = hex_float(&USART_RX_Buffer[9]);
            Re_inter->float_2 = hex_float(&USART_RX_Buffer[13]);
            Re_inter->float_3 = hex_float(&USART_RX_Buffer[17]);
            Re_inter->float_4 = hex_float(&USART_RX_Buffer[21]);
            Re_inter->float_5 = hex_float(&USART_RX_Buffer[25]);
            Re_inter->float_6 = hex_float(&USART_RX_Buffer[29]);
            Re_inter->float_7 = hex_float(&USART_RX_Buffer[33]);
            Re_inter->float_8 = hex_float(&USART_RX_Buffer[37]);
            Re_inter->float_9 = hex_float(&USART_RX_Buffer[41]);
            Re_inter->float_10 = hex_float(&USART_RX_Buffer[45]);
            Re_inter->float_11 = hex_float(&USART_RX_Buffer[49]);


            Frame_Head_sta=0;//数据处理完毕，帧头帧尾标志位置0
            Frame_End_sta=0;
        }
    }
    else{
        memset( USART_RX_Buffer,0x00,USART_RX_LEN);//全部清零，重新接收
        Frame_Head_sta=0;//帧头帧尾标志位全部置0
        Frame_End_sta=0;
    }
}

/*-----------以下为比特和其他类型相互转化的函数  主要看上面的俩函数-------------*/

//单片机发数据时用--------------------------------------------------
void Int_to_Byte(int i,unsigned char *byte)
{

    unsigned long longdata = 0;
    longdata = *(unsigned long*)&i;
    byte[3] = (longdata & 0xFF000000) >> 24;
    byte[2] = (longdata & 0x00FF0000) >> 16;
    byte[1] = (longdata & 0x0000FF00) >> 8;
    byte[0] = (longdata & 0x000000FF);

}
void Float_to_Byte(float f,unsigned char *byte)
{

    unsigned long longdata = 0;
    longdata = *(unsigned long*)&f;
    byte[3] = (longdata & 0xFF000000) >> 24;
    byte[2] = (longdata & 0x00FF0000) >> 16;
    byte[1] = (longdata & 0x0000FF00) >> 8;
    byte[0] = (longdata & 0x000000FF);

}

void Short_to_Byte(short s,unsigned char *byte)
{
    byte[1] = (s & 0xFF00) >> 8;
    byte[0] = (s & 0xFF);
}
//-----------------------------------------------------------------




//单片机接收数据时用--------------------------------------------------
float hex_float(uint8_t *a)//十六进制数转浮点数,参数为4个八位二进制数
{
    uint8_t b,c,d;
    b=*(a+1);
    c=*(a+2);
    d=*(a+3);
    union _float   //定义联合体，十六进制和浮点数地址相同
    {

        float f;
        uint8_t byte[4];
    };
    float m;
    union _float fl;
    fl.byte[0]=*a;
    fl.byte[1]=b;
    fl.byte[2]=c;
    fl.byte[3]=d;
    m=fl.f;
    return m;
}

short int hex_sint(uint8_t *a)//十六进制数转短整型数,参数为4个八位二进制数
{
    uint8_t b=*(a+1);
    union _int
    {
        short int _i;
        uint8_t byte[2];
    };
    short int m;
    union _int i;
    i.byte[0]=*a;
    i.byte[1]=b;
    m=i._i;
    return m;
}
//-----------------------------------------------------------------

uint8_t uart2_rx_data,Frame_Head_sta,Frame_End_sta;
uint8_t uart_data,uart_flag;
int cnt_isr;
//extern unsigned char USART_RX_Buffer[USART_RX_LEN];
//void EUSCIA2_IRQHandler(void)
//{
//    while(UART_query_byte(EUSCI_A2_BASE, &uart2_rx_data))
//    {
//
//        //透传模式 用户在此处接收配对的蓝牙发送过来的额数据
//        //读取无线串口的数据 并且置位接收标志
//        uart_flag = 1;
//        uart_data = uart2_rx_data;
//
//        if(uart_data==0xa5 && Frame_End_sta==0)//如果接收到数据是帧头并且帧尾标志位为0
//            Frame_Head_sta=1;//此时接收到为帧头，帧头标志位置1
//        if(uart_data==0x5a && Frame_Head_sta==1) //如果已经接收到帧头，且此时接收到数据为0x5A
//            Frame_End_sta=1;//判定此次为帧尾
//
//        if(Frame_Head_sta==1)//已经接收到帧头
//        {
//            USART_RX_Buffer[cnt_isr]=uart_data;//把接收到的数据写进数组
//            cnt_isr++;
//
//            if(cnt_isr==USART_RX_LEN)//如果i=接收数组长度，说明一帧数据已经接收完成
//            {
//                if(uart_data!=0x5a)//如果最后一个不是帧尾，说明接收错误
//                {
//                    Frame_Head_sta=0;//帧头帧尾标志位全部置0，重新接收
//                    Frame_End_sta=0;
//                }
//
//                cnt_isr=0;
//            }
//        }
//        if(USART_RX_Buffer[0]!=0xa5)//如果第一个数据不是帧头，说明接收错误
//        {
//
//            cnt_isr=0;
//            Frame_Head_sta=0;//帧头帧尾标志位全部置0，重新接收
//            Frame_End_sta=0;
//        }
//    }
//}

