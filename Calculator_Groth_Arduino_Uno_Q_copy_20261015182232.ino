void setup()
{
    Serial.begin(9600);
}

void loop()
{
    if (Serial.available())
    {
        String input = Serial.readStringUntil('\n');

        int a, b;
        char op;

        if (sscanf(input.c_str(), "%d %c %d", &a, &op, &b) == 3)
        {
            long result = 0;
            bool valid = true;

            switch(op)
            {
                case '+':
                    result = a + b;
                    break;

                case '-':
                    result = a - b;
                    break;

                case '*':
                    result = a * b;
                    break;

                case '/':
                    if(b != 0)
                        result = a / b;
                    else
                    {
                        Serial.println("Fehler: Division durch 0");
                        valid = false;
                    }
                    break;

                default:
                    Serial.println("Fehler: Operator");
                    valid = false;
            }

            if(valid)
            {
                Serial.println(result);
            }
        }
        else
        {
            Serial.println("Fehler: Ungueltiger Term");
        }
    }
}