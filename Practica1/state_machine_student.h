



/********************************************************
 *                                                      *
 *                                                      *
 *      state_machine_student.h          		*
 *                                                      *
 *		Student:				*
 *		FI-UNAM					*
 *		2-15-2024                               *
 *                                                      *
 ********************************************************/

// Student State Machine
AdvanceAngle reactive_students(Raw observations, int dest, int intensity, float Mag_Advance, float max_angle, int num_sensors)
{

        AdvanceAngle gen_vector;
        int obs;
        int j;
        float left_side = 0;
        float right_side = 0;
        int value = 0;
        static int step = 0;

        step++;
        printf("\n\n **************** Student Reactive Behavior %d *********************\n", step);

        for (j = 0; j < num_sensors / 2; j++)
        {
                right_side = observations.sensors[j] + right_side;
                printf("right side sensor[%d] %f\n", j, observations.sensors[j]);
        }

        for (j = num_sensors / 2; j < num_sensors; j++)
        {
                left_side = observations.sensors[j] + left_side;
                printf("left side sensor[%d] %f\n", j, observations.sensors[j]);
        }

        right_side = right_side / (num_sensors / 2);
        left_side = left_side / (num_sensors / 2);
        printf("Average right side %f\n", right_side);
        printf("Average left side %f\n", left_side);

        if (left_side < THRS)
                value = (value << 1) + 1;
        else
                value = (value << 1) + 0;

        if (right_side < THRS)
                value = (value << 1) + 1;
        else
                value = (value << 1) + 0;

        obs = value;
        printf("intensity %d obstacles %d dest %d\n", intensity, obs, dest);

        if (intensity == 1)
        {
                // Constants STOP, TURN RIGHT, ETC, are defined in ../utilities/constants.h
                // generate_output function in ../utilities/utilities.h
                gen_vector = generate_output(STOP, Mag_Advance, max_angle);
                printf("STOP\n");
                printf("\n **************** Reached light source ******************************\n");
        }
        else if (obs == 0)
        {
                // There is not obstacle
                gen_vector = generate_output(FORWARD, Mag_Advance, max_angle);
                printf("FORWARD\n");
        }
        else if (obs == 1)
        {
                // Obtacle in the right
                gen_vector = generate_output(LEFTADVANCE, Mag_Advance, max_angle);
                gen_vector = generate_output(FORWARD, Mag_Advance, max_angle);
                printf("TURN LEFT\n");
        }
        else if (obs == 2)
        {
                // obtacle in the left
                gen_vector = generate_output(RIGHTADVANCETWICE, Mag_Advance, max_angle);
                printf("TURN RIGHT\n");
        }
        else if (obs == 3)
        {
                // obstacle in the front
                gen_vector = generate_output(LEFTADVANCETWICE, Mag_Advance, max_angle);
                printf("TURN LEFT\n");
        }

        return gen_vector;
}

// Student State Machine
AdvanceAngle BUG1(coord coord_robot, coord coord_dest, Raw observations, int dest, int intensity, int state, int *next_state, float Mag_Advance, float max_angle, int num_sensors)
{

        AdvanceAngle gen_vector;
        int obs;
        int j;
        float left_side = 0;
        float right_side = 0;
        int value = 0;

        printf("\n\n **************** Student State Machine *********************\n");

        for (j = 0; j < num_sensors / 2; j++)
        {
                right_side = observations.sensors[j] + right_side;
                printf("right side sensor[%d] %f\n", j, observations.sensors[j]);
        }

        for (j = num_sensors / 2; j < num_sensors; j++)
        {
                left_side = observations.sensors[j] + left_side;
                printf("left side sensor[%d] %f\n", j, observations.sensors[j]);
        }

        right_side = right_side / (num_sensors / 2);
        left_side = left_side / (num_sensors / 2);
        printf("Average right side %f\n", right_side);
        printf("Average left side %f\n", left_side);

        if (left_side < THRS)
                value = (value << 1) + 1;
        else
                value = (value << 1) + 0;

        if (right_side < THRS)
                value = (value << 1) + 1;
        else
                value = (value << 1) + 0;

        obs = value;
        printf("intensity %d obstacles %d dest %d\n", intensity, obs, dest);

        switch (state)
        {

        case 0:
                if (intensity == 1)
                {
                        gen_vector = generate_output(STOP, Mag_Advance, max_angle);
                        *next_state = 1;

                        printf("Present State: %d STOP\n", state);
                        printf("\n **************** Reached light source ******************************\n");
                }
                else
                {

                        gen_vector = generate_output(FORWARD, Mag_Advance, max_angle);
                        *next_state = 1;

                        printf("Present State: %d FORWARD\n", state);
                }

                break;

        case 1:
                while (obs == 0)
                {
                        // No hay obstáculo, avanzar hacia adelante
                        gen_vector = generate_output(FORWARD, Mag_Advance, max_angle);
                        printf("Present State: %d FORWARD\n", state);
                       
                }

                // Cuando hay un obstáculo
                gen_vector = generate_output(STOP, Mag_Advance, max_angle);
                printf("Present State: %d STOP\n", state);

                if (obs == 1) // Obstáculo a la derecha
                {
                        
                        *next_state = 2; //girar izq
                }
                else if (obs == 2) // Obstáculo a la izquierda
                {
                        
                        *next_state = 3;//girar der
                }
                else if (obs == 3)  // Obstáculo al frente
                {
                       
                        *next_state = ;
                }
                else if (obs=3)
                {
                        if(right_side>left_side) //Obstaculo arriba a la izq
                        {
                                *next_state= 3; //girar der
                        }
                        else
                        {
                            *next_state=2;     //girar izq
                        }
                }
                



                break;

        case 2:  //GIRAR IZQ AL VER UN OBSTACULO A LA DERECHA (EVADIR)
                  gen_vector = generate_output(LEFT, Mag_Advance, max_angle); 
                  *next_state= 6;
                  break;               

        case 3: //GIRAR DER AL VER UN OBS A LA IZQ (EVADIR)
                  gen_vector = generate_output(RIGHT, Mag_Advance, max_angle);
                  *next_state=6;
                  break;

        case 4: //REGRESAR Y GIRAR DER SI HAY OBS ENFRENTE (EVADIR)
                 gen_vector = generate_output(BACKWARD, Mag_Advance, max_angle);
                 gen_vector = generate_output(RIGHT, Mag_Advance, max_angle);
                  *next_state=6;
                 break;

        case 5: //REGRESAR Y GIRAR IZQ SI HAY OBS ENFRENTE (EVADIR)

                 gen_vector = generate_output(BACKWARD, Mag_Advance, max_angle);
                 gen_vector = generate_output(RIGHT, Mag_Advance, max_angle);
                 *next_state=6;
                 break;

        case 6: //SEGUIR ENFRENTE (CAMBIO DE ETAPA)
                   gen_vector = generate_output(BACKWARD, Mag_Advance, max_angle);
                   *next_state= 7;
                   break;
                   
        case 7:  //ENCONTRAR EL OBJETO PARA RODEARLO

                gen_vector = generate_output(STOP, Mag_Advance, max_angle);
                printf("Present State: %d STOP\n", state);

                if (obs == 1) // Obstáculo a la derecha
                {
                        
                        *next_state = 8; //girar der
                }
                else if (obs == 2) // Obstáculo a la izquierda
                {
                        
                        *next_state = 9 ;//girar izq
                }
              
                else if (obs=3)
                {
                        if(right_side>left_side) //Obstaculo arriba a la izq
                        {
                          *next_state= 11; //girar izq
                        }
                        else
                        {
                          *next_state= 10;    //girar der
                        }
                }

                break;

        case 8: //girar der (RODEAR)
         gen_vector = generate_output(RIGHT, Mag_Advance, max_angle);
         *next_state=0;

        case 9:  //girar izq (RODEAR)
         gen_vector = generate_output(LEFT, Mag_Advance, max_angle);
          *next_state=0;

        case 10: // AVANZAR Y GIRAR PARA RODEAR A LA DER
         gen_vector = generate_output(FORWARD, Mag_Advance, max_angle);
          gen_vector = generate_output(RIGHT, Mag_Advance, max_angle);
           *next_state=0;

        case 11: // AVANZAR Y GIRAR PARA RODEAR A LA IZQ
         gen_vector = generate_output(FORWARD, Mag_Advance, max_angle);
          gen_vector = generate_output(LEFT, Mag_Advance, max_angle);
           *next_state=0;
                





        default:
                printf("State %d not defined used ", state);
                gen_vector = generate_output(STOP, Mag_Advance, max_angle);
                next_state = 0;
                break;
        }

        return gen_vector;
}
