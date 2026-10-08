/* ========================================
 *
 * Harold Araya October, 2026
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include <stdio.h> // Allows for sprintf function
// Navy Temperature Flags Reader (Wet Bulb Globe Temperature Flag System)

/*
    We need 5 flags, So 5 conditions of temperatures being read.
    1. ALL LED OFF == White Flag --> Low risk; minimal restrictions, encourage hydration.
    2. Green LED ON == Green Flag == (80 - 84.9) --> Moderate risk; use caution and supervision for heavy exercise or unacclimated personnel.
    3. Green + Red LED ON == Yellow Flag ==(85 - 87.9) -->  High risk; curtail strenuous exercise and outdoor activities for new or unacclimated personnel.
    4. RED LED ON == Red Flag == (88 - 89.9) --> Very high risk; suspend strenuous exercise for personnel with under 12 weeks of hot-weather training.
    5. ALL LEDs ON == Black Flag == (90 - Above) --> Extreme risk; all non-essential physical training and strenuous outdoor activity are halted.
    
    Modified the Build settings under project. Went into ARM GCC and changed the Linker --> 
    General setting making "Use newlib-nano Float Formatting set to True"
    then went into Temp_Reading.cydwr
    changed heap size from 0x80 to 0x200
    gives sprintf enough memory to process floating-point math!
*/
    
int main(void)
{
    uint8 rawAdcValue = 0;
    uint8 initialAdcValue = 0;
    float actualTemp = 0.0;
    uint8 isStarted = 0; //Tracks knob turn
    uint16 scrollCount = 0;
    
    // Instead of CyDelay(); we can use a flashTimer to blink the LEDs
    uint16 flashTimer = 0;
    char LCDbuffer[16];
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    LCD_Start();
    ADC_SAR_1_Start();
    ADC_SAR_1_StartConvert(); // This allows for Knob to continue read pin
 
    LCD_Position(0,1);
    LCD_PrintString("Navy Wet Bulb Flag Sys");
    LCD_Position(1,0);
    LCD_PrintString("Turn Knob To Start");
    
    // Baseline of the knob starting
    while(!ADC_SAR_1_IsEndConversion(ADC_SAR_1_WAIT_FOR_RESULT));
    initialAdcValue = ADC_SAR_1_GetResult8();
    
    for(;;)
    {
        if(ADC_SAR_1_IsEndConversion(ADC_SAR_1_WAIT_FOR_RESULT))
        {
            rawAdcValue = ADC_SAR_1_GetResult8(); 
        }
        
        // Check knob movement:
        if(!isStarted)
        {
            if(scrollCount >= 250)
            {
                LCD_WriteControl(LCD_DISPLAY_SCRL_LEFT);
                scrollCount = 0;
            }
            scrollCount++;
            
            // Checks if knob is turned clockwise or counter clockwise with a +-3 threshold filter to minimize noise on the knob
            if((rawAdcValue > initialAdcValue + 3) || (rawAdcValue < initialAdcValue - 3))
            {
                LCD_ClearDisplay();
                isStarted = 1; // Transitions to live monitoring
            }
        }
        // Create a started system (once knob is turned) (switches the LCD screen to temp screen)
        if(isStarted)
        {
            actualTemp = 75.0 + (((float)rawAdcValue * 30.0) / 255.0);
        
            LCD_Position(0,0);
            //LCD_PrintString("Temp: ");
            sprintf(LCDbuffer, "Temp:%.1fF     ", actualTemp);
            LCD_PrintString(LCDbuffer);
        
            flashTimer++;
        
            // Add LED functionality as specified above!
            if(actualTemp >= 90.0)
            {
                if(flashTimer >= 25)
                {
                //Black Flag req! Flashing Lights DANGER
                GreenLED_Write(!GreenLED_ReadDataReg());
                RedLED_Write(!RedLED_ReadDataReg());             
                LED3_Write(!LED3_ReadDataReg());               
                LED4_Write(!LED4_ReadDataReg());      
                flashTimer = 0;
                }
                LCD_Position(1,0);
                LCD_PrintString("Black Flag!        ");        
            }
            else if (actualTemp >= 88.0)
            {
                //Red Flag req!
                GreenLED_Write(0);
                RedLED_Write(1);
                LED3_Write(0);
                LED4_Write(0);
                LCD_Position(1,0);
                LCD_PrintString("Red Flag!      ");
            }
            else if (actualTemp >= 85.0)
            {
                if(flashTimer >= 250)
                {
                //Yellow Flag req! Have both green and red flash since no yellow LED is provided
                GreenLED_Write(!GreenLED_ReadDataReg());
                RedLED_Write(!RedLED_ReadDataReg());
                flashTimer = 0;
                }
                LED3_Write(0);
                LED4_Write(0);
                LCD_Position(1,0);
                LCD_PrintString("Yellow Flag!       ");
                
            }
            else if (actualTemp >= 80.0)
            {
                //Green Flag req!
                GreenLED_Write(1);
                RedLED_Write(0);
                LED3_Write(0);
                LED4_Write(0);
                LCD_Position(1,0);
                LCD_PrintString("Green Flag!        ");
            }
            else
            {
                //White Flag req!
                GreenLED_Write(0);
                RedLED_Write(0);
                LED3_Write(0);
                LED4_Write(0);
                LCD_Position(1,0);
                LCD_PrintString("White Flag!        ");
            }
        
        }
        CyDelay(1);
    }
}

/* [] END OF FILE */
