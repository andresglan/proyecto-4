/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
uint16 dato,cont;
volatile uint16 t,h,n,i;
volatile uint8 size;
char Mensaje;

//#define tiempoTemp 3

CY_ISR(Int_Timer){
    Timer_1_Sleep();
    
   
            
            
            size=UART_BL_GetRxBufferSize(); //Leer la cantidad de datos recibidos
            
            UART_BL_PutString("T,");
            dato=t/100;
            t=t%100;
            UART_BL_PutChar(dato+0x30); 
            dato=t/10;
            t=t%10;
            UART_BL_PutChar(dato+0x30);
            
            UART_BL_PutString(",");
            /////HUMEDAD
            UART_BL_PutString("H,");
            dato=(h%100)/10;
          
            UART_BL_PutChar(dato+0x30); 
            dato=h%10;
            UART_BL_PutChar(dato+0x30);
            
            UART_BL_PutString(",");
            /////NIVEL
            UART_BL_PutString("N,");
            dato=n/100;
            UART_BL_PutChar(dato+0x30);
            dato=(n%100)/10;
            UART_BL_PutChar(dato+0x30); 
            dato=n%10;
            UART_BL_PutChar(dato+0x30);
            
            UART_BL_PutString(",");
            
            if(size>0){ //Si se recibieron caracteres se verifica si se solcitó riego
                
                    Mensaje=UART_BL_GetChar();
                    
                    
            }
            h=h-100;
            if(Mensaje==78){
                BOMBA_Write(1);
            
            }else if(h<40){
                    BOMBA_Write(1);
                    
                }else{
                    BOMBA_Write(0);
                }
            
            
//            if (strcmp(Mensaje,"ROF")==0){
//                    BOMBA_Write(0);
//                    LCD_Position(1,10);
//                    LCD_PrintNumber(size);
//                }else if(strcmp(Mensaje,"RON")==0){
//                    BOMBA_Write(1);
//                    LCD_Position(1,10);
//                    LCD_PrintNumber(size);
//                }
            
            
            
            
            
    Timer_1_Wakeup();     
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    ISR_Timer_StartEx(Int_Timer);
    ADC_Start();
    LCD_Start();
    ADC2_Start();
    VDAC_Start();
    VDAC2_Start();
    UART_BL_Start();
    Timer_1_Start();
    for(;;)
    {
        cont++;    
        t=0;
        h=0;
        n=0;
        t=ADC2_Read16(); //Inicia conversión, espera final de conversión y genera resultado
        t=t*1526/1000; 
    
        for(i=0;i<15;i++){
            ADC_StartConvert(); //Iniciar conversión        
            ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);  //Esperar final de conversión
            h=h+ADC_GetResult16(0);
            n=n+ADC_GetResult16(1);
              
        }
        ADC_StopConvert();
        t=t/100;
        h=h/15;
        n=n/15;
        h=ADC_CountsTo_mVolts(h);
        n=ADC_CountsTo_mVolts(n);
        n=n/10;
        

      
        h=5000-h;
        
        if(n>250||n<100){
            //LCD_Position(0,5);
            //LCD_PrintString("N bajo");
            n=100;
            
            VDAC2_SetValue(n);
        }
//        }else{
//            
//            n=n+1000;
//            n=(0.0569*n)+75.11;
//            
//            
//            
//            
//        }
//        if(n>255){n=255;}
        VDAC2_SetValue((uint8)n);
        h=(h*100)/4600;
        if(h>100){
         h=10;
        }
        
        LCD_Position(0,1),
        LCD_PrintNumber(t);
        LCD_Position(0,7);
        LCD_PrintNumber(n);
        LCD_Position(1,1);
        LCD_PrintNumber(h);
        h=h+100;
        
        LCD_PrintString("%");
        
        
        
       
        
        
        
        
        
//        if(cont==tiempoTemp){UART_BL_PutString("Q,");}
//        dato=t/100;
//        t=t%100;
//        LCD_PrintNumber(dato);
//        if(cont==tiempoTemp){UART_BL_PutChar(dato+0x30);}
//        dato=t/10;
//        t=t%10;
//        LCD_PrintNumber(dato);
//        
//        
//        
//        if(cont==tiempoTemp){
//            UART_BL_PutChar(dato+0x30);
//            UART_BL_PutString(",\r\n");
//            cont=0;
//        }
        
        CyDelay(1000); 
    }
}

/* [] END OF FILE */
