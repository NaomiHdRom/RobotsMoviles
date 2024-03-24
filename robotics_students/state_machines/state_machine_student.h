
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

        float dist_goal;
        float dist_min;

        coord coord_ini;
        coord pos_rodeo;
        coord pos_min;

        bool rodeo = false;
        int contador;

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
                if (obs == 0)
                {
                        // There is not obstacle
                        gen_vector = generate_output(FORWARD, Mag_Advance, max_angle);
                        *next_state = 13;

                        printf("Present State: %d FORWARD\n", state);
                }
                else
                {
                        gen_vector = generate_output(STOP, Mag_Advance, max_angle);
                        printf("Present State: %d STOP\n", state);
                        coord_ini = coord_robot;
                        pos_rodeo = coord_robot;
                        pos_min = coord_robot;
                        dist_min = distance(pos_min, coord_dest);

                        if (obs == 1)
                        {
                                // obtacle in the  right
                                *next_state = 9;
                                contador++;
                                printf("contador actual= %d", contador);
                        }
                        else if (obs == 2)
                        {
                                // obtacle in the left
                                *next_state = 12;
                                contador++;
                                printf("contador actual= %d", contador);
                        }
                        else if (obs == 3)
                        {
                                contador++;
                                printf("contador actual= %d", contador);

                                if (left_side < right_side) //esquina en la izq
                                {
                                        *next_state = 4;
                                }
                                else if (left_side > right_side) //esquina en la der
                                {
                                        *next_state = 2;
                                }
                                else //obs enfrente
                                {
                                        *next_state = 6;
                                }
                        }
                }

                break;

        case 2: // Backward, obstacle in the right
                gen_vector = generate_output(BACKWARD, Mag_Advance, max_angle);
                contador--;
                printf("contador actual= %d", contador);
                pos_rodeo = coord_robot;
                dist_goal = distance(pos_rodeo, coord_dest);

                if (dist_min > dist_goal)
                {
                        pos_min = coord_robot;
                        dist_min = distance(pos_min, coord_dest);
                }

                *next_state = 3;

                printf("Present State: %d BACKWARD, obstacle right\n", state);
                break;

        case 3: // right turn
                gen_vector = generate_output(LEFT, Mag_Advance, max_angle);
                *next_state = 10;

                printf("Present State: %d TURN LEFT\n", state);
                break;

        case 4: // Backward, obstacle in the left
                gen_vector = generate_output(BACKWARD, Mag_Advance, max_angle);
                contador--;
                printf("contador actual= %d", contador);
                pos_rodeo = coord_robot;
                dist_goal = distance(pos_rodeo, coord_dest);

                if (dist_min > dist_goal)
                {
                        pos_min = coord_robot;
                        dist_min = distance(pos_min, coord_dest);
                }
                *next_state = 5;

                printf("Present State: %d BACKWARD, obstacle left\n", state);
                break;

        case 5: // left turn
                gen_vector = generate_output(RIGHT, Mag_Advance, max_angle);
                *next_state = 10;

                printf("Present State: %d TURN RIGTH\n", state);
                break;

        case 6: // Backward, obstacle in front
                gen_vector = generate_output(BACKWARD, Mag_Advance, max_angle);
                contador--;
                printf("contador actual= %d", contador);
                pos_rodeo = coord_robot;
                dist_goal = distance(pos_rodeo, coord_dest);

                if (dist_min > dist_goal)
                {
                        pos_min = coord_robot;
                        dist_min = distance(pos_min, coord_dest);
                }
                *next_state = 7;

                printf("Present State: %d BACKWARD, obstacle FRONT\n", state);
                break;

        case 7: /// Left turn
                gen_vector = generate_output(LEFT, Mag_Advance, max_angle);
                *next_state = 8;

                printf("Present State: %d TURN 1 LEFT\n", state);
                break;

        case 8: // Left turn
                gen_vector = generate_output(LEFT, Mag_Advance, max_angle);
                *next_state = 9;

                printf("Present State: %d TURN 2 LEFT\n", state);
                break;

        case 9: // Forward
                gen_vector = generate_output(FORWARD, Mag_Advance, max_angle);
                contador++;
                printf("contador actual= %d", contador);

                *next_state = 10;

                printf("Present State: %d 1 FORWARD\n", state);
                break;

        case 10: //CERRAR RODEO
                if (coord_ini.xc == coord_robot.xc && coord_ini.yc == coord_robot.yc && coord_ini.anglec == coord_robot.anglec)
                {
                        rodeo = true;
                        printf("RODEO: TRUE");
                        pos_min = coord_robot;

                        gen_vector = generate_output(STOP, Mag_Advance, max_angle);

                        *next_state = 19;
                }
                else
                {
                        gen_vector = generate_output(STOP, Mag_Advance, max_angle);
                        gen_vector = generate_output(STOP, Mag_Advance, max_angle);
                        gen_vector = generate_output(STOP, Mag_Advance, max_angle);

                        *next_state = 11;
                }

                break;

        case 11:
                if (obs == 0)
                {
                        gen_vector = generate_output(RIGHT, Mag_Advance, max_angle);
                        *next_state = 16;
                }
                else
                {
                        gen_vector = generate_output(STOP, Mag_Advance, max_angle);
                        coord_ini = coord_robot;
                        if (obs == 1)
                        {
                                *next_state = 9;
                        }
                        else if (obs == 2)
                        {
                                *next_state = 12;
                        }
                        else if (obs == 3)
                        {
                                if (left_side < right_side)
                                {
                                        *next_state = 4;
                                }
                                else if (left_side > right_side)
                                {
                                        *next_state = 2;
                                }
                        }
                        else
                        {
                                *next_state = 6;
                        }
                }

                break;

        case 12: // Right turn
                gen_vector = generate_output(RIGHT, Mag_Advance, max_angle);
                pos_rodeo = coord_robot;
                dist_goal = distance(pos_rodeo, coord_dest);

                if (dist_min > dist_goal)
                {
                        pos_min = coord_robot;
                        dist_min = distance(pos_min, coord_dest);
                }
                *next_state = 14;

                printf("Present State: %d turn 2 RIGHT\n", state);
                break;

        case 13: // // check destination
                if (dest == 0)
                {
                        // go right
                        gen_vector = generate_output(RIGHT, Mag_Advance, max_angle);
                        *next_state = 17;

                        printf("Present State: %d RIGHT\n", state);
                }
                else if (dest == 1)
                {
                        // go left
                        gen_vector = generate_output(LEFT, Mag_Advance, max_angle);
                        *next_state = 18;

                        printf("Present State: %d LEFT\n", state);
                }
                else if (dest == 2)
                {
                        // go right single
                        gen_vector = generate_output(FORWARD, Mag_Advance, max_angle);
                        contador++;
                        printf("contador actual= %d", contador);
                        *next_state = 17;

                        printf("Present State: %d FORWARD\n", state);
                }
                else if (dest == 3)
                {
                        // go left single
                        gen_vector = generate_output(FORWARD, Mag_Advance, max_angle);
                        *next_state = 18;

                        printf("Present State: %d FORWARD\n", state);
                }
                break;

        case 14:
                gen_vector = generate_output(RIGHT, Mag_Advance, max_angle);
                *next_state = 15;
                break;

        case 15:
                gen_vector = generate_output(RIGHT, Mag_Advance, max_angle);
                *next_state = 16;
                break;

        case 16:
                gen_vector = generate_output(RIGHT, Mag_Advance, max_angle);
                *next_state = 9;
                break;

        case 17: //detener
                gen_vector = generate_output(RIGHT, Mag_Advance, max_angle);
                *next_state = 0;
                break;

        case 18:
                gen_vector = generate_output(LEFT, Mag_Advance, max_angle);
                *next_state = 0;
                break;

        case 19: //CONFIRMAR REGRESO
                if (rodeo == true && pos_min.xc == coord_robot.xc && pos_min.yc == coord_robot.yc && pos_min.anglec == coord_robot.anglec)
                {
                        gen_vector = generate_output(STOP, Mag_Advance, max_angle);
                        *next_state = 13;
                }

                break;

        default:
                printf("State %d not defined used ", state);
                gen_vector = generate_output(STOP, Mag_Advance, max_angle);
                *next_state = 0;
                break;
        }

        printf("CONTADOR DE PASOS= %d", contador);

        return gen_vector;
}
