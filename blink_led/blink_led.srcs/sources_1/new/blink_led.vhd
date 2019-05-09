----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 09.05.2019 11:28:15
-- Design Name: 
-- Module Name: blink_led - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity blink_led is
    Port ( CLK : in STD_LOGIC;
           RESET : in STD_LOGIC;
           LED : out STD_LOGIC_VECTOR (1 downto 0);
           LED2 : out STD_LOGIC_VECTOR (1 downto 0);
           SW : in STD_LOGIC);
end blink_led;

architecture Behavioral of blink_led is

--signal counter: unsigned(NUMBER_OF_BITS-1 downto 0);
signal counter: unsigned(28 downto 0);
signal counter2: unsigned(26 downto 0);

begin

COUNTER_PROC : process (CLK)
begin

    if rising_edge(CLK) then
        if RESET = '1' then
            counter <= (others => '0');
        elsif SW = '1' then
            counter <= counter + 1;
        elsif SW = '0' then
            counter <= counter - 1;
        else
            counter <= counter;
        end if;
    end if;
    
end process COUNTER_PROC;


COUNTER_PROC_2 : process (CLK)
begin

    if rising_edge(CLK) then
        if RESET = '1' then
            counter2 <= (others => '0');
        elsif SW = '1' then
            counter2 <= counter2 + 1;
        elsif SW = '0' then
            counter2 <= counter2 - 1;
        else
            counter2 <= counter2;
        end if;
    end if;
    
end process COUNTER_PROC_2;

LED <= std_logic_vector(counter(28 downto 27));
LED2 <= std_logic_vector(counter2(26 downto 25));

end Behavioral;
