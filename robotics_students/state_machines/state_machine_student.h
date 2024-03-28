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
AdvanceAngle reactive_students(Raw observations, int dest, int intensity, float Mag_Advance, float max_angle, int num_sensors){

 AdvanceAngle gen_vector;
 int obs;
 
 int j;
 float left_side=0;
 float right_side=0;
 int value = 0;
 static int step=0;

 step++;
 printf("\n\n **************** Reactive Behavior %d *********************\n",step);

 for(j=0;j<num_sensors/2;j++){
        right_side = observations.sensors[j] + right_side;
        printf("right side sensor[%d] %f\n",j,observations.sensors[j]);
 }

 for(j=num_sensors/2;j<num_sensors;j++){
        left_side = observations.sensors[j] + left_side;
        printf("left side sensor[%d] %f\n",j,observations.sensors[j]);
 }

 right_side = right_side/(num_sensors/2);
 left_side = left_side/(num_sensors/2);
 printf("Average right side %f\n",right_side);
 printf("Average left side %f\n",left_side);

 if( left_side < THRS) value = (value << 1) + 1;
 else value = (value << 1) + 0;

 if( right_side < THRS) value = (value << 1) + 1;
 else value = (value << 1) + 0;

 obs = value;
 printf("intensity %d obstacles %d dest %d\n",intensity,obs,dest);

 if (intensity == 1){
	// Constants STOP, TURN RIGHT, ETC, are defined in ../utilities/constants.h
	// generate_output function in ../utilities/utilities.h
	gen_vector=generate_output(STOP,Mag_Advance,max_angle);
        printf("STOP\n");
	printf("\n **************** Reached light source ******************************\n");
 }
 
 else if (obs == 1){
        // Obtacle in the right
        gen_vector=generate_output(FORWARD,Mag_Advance,max_angle);
	printf("TURN LEFT\n");
 }
 else if (obs == 2){
        // obtacle in the left
	gen_vector=generate_output(RIGHTADVANCETWICE,Mag_Advance,max_angle);
	printf("TURN RIGHT\n");
 }
 else if (obs == 3){
	// obstacle in the front
        gen_vector=generate_output(LEFTADVANCETWICE,Mag_Advance,max_angle);
	printf("TURN LEFT\n");
 }
else if (obs == 0){
	// There is not obstacle
        gen_vector=generate_output(FORWARD,Mag_Advance,max_angle);
	printf("FORWARD\n");

	
 }


 return gen_vector;

}



// Student State Machine 
AdvanceAngle state_machine_students(Raw observations, int dest, int intensity, int state, int *next_state, float Mag_Advance, float max_angle, int num_sensors){

 AdvanceAngle gen_vector;
 int obs;
 int j;
 float left_side=0;
 float right_side=0;
 int value = 0;

 printf("\n\n **************** Student State Machine *********************\n");

 for(j=0;j<num_sensors/2;j++){
        right_side = observations.sensors[j] + right_side;
        printf("right side sensor[%d] %f\n",j,observations.sensors[j]);
 }

 for(j=num_sensors/2;j<num_sensors;j++){
        left_side = observations.sensors[j] + left_side;
        printf("left side sensor[%d] %f\n",j,observations.sensors[j]);
 }

 right_side = right_side/(num_sensors/2);
 left_side = left_side/(num_sensors/2);
 printf("Average right side %f\n",right_side);
 printf("Average left side %f\n",left_side);

 if( left_side < THRS) value = (value << 1) + 1;
 else value = (value << 1) + 0;

 if( right_side < THRS) value = (value << 1) + 1;
 else value = (value << 1) + 0;

 obs = value;
 printf("intensity %d obstacles %d dest %d\n",intensity,obs,dest);

 switch ( state ) {

        case 0:
                if (intensity == 1){
                        gen_vector=generate_output(STOP,Mag_Advance,max_angle);
                        *next_state = 1;

                        printf("Present State: %d STOP\n", state);
			printf("\n **************** Reached light source ******************************\n");
                }
                else{

			gen_vector=generate_output(FORWARD,Mag_Advance,max_angle);
                        *next_state = 1;

                        printf("Present State: %d FORWARD\n", state);
                }

                break;

        case 1:
                if (obs == 0){
			// There is not obstacle
                        gen_vector=generate_output(FORWARD,Mag_Advance,max_angle);
                        *next_state = 1;

                        printf("Present State: %d FORWARD\n", state);
                }
                else{
                        gen_vector=generate_output(STOP,Mag_Advance,max_angle);
                        printf("Present State: %d STOP\n", state);

                        if (obs == 1){
                                // obtacle in the  right
                                *next_state = 2;
                        }
                        else if (obs == 2){
                                // obtacle in the left
                                *next_state = 4;
                        }
                        else if (obs == 3){
				// obstacle in the front
                                *next_state = 7;
                        }
                }

                break;

        case 2: // Backward, obstacle in the right
                gen_vector=generate_output(FORWARD,Mag_Advance,max_angle);
                *next_state = 3;

		printf("Present State: %d BACKWARD, obstacle right\n", state);
                break;

        case 3: // right turn
                gen_vector=generate_output(FORWARD,Mag_Advance,max_angle);
                *next_state = 0;

		printf("Present State: %d TURN LEFT\n", state);
                break;

        case 4: // Backward, obstacle in the left
                gen_vector=generate_output(BACKWARD,Mag_Advance,max_angle);
                *next_state = 5;

		printf("Present State: %d BACKWARD, obstacle left\n", state);
                break;

        case 5: // left turn
                gen_vector=generate_output(RIGHT,Mag_Advance,max_angle);
                *next_state = 0;

		printf("Present State: %d TURN RIGTH\n", state);
                break;

        case 6: // Backward, obstacle in front
                gen_vector=generate_output(LEFT,Mag_Advance,max_angle);
                *next_state = 7;

		printf("Present State: %d BACKWARD, obstacle FRONT\n", state);
                break;

	case 7: /// Left turn
                gen_vector=generate_output(LEFT,Mag_Advance,max_angle);
                *next_state = 8;

		printf("Present State: %d TURN 1 LEFT\n", state);
                break;

        case 8:// Left turn
                gen_vector=generate_output(LEFT,Mag_Advance,max_angle);
                *next_state = 9;

		printf("Present State: %d TURN 2 LEFT\n", state);
                break;

        case 9: // Forward
                gen_vector=generate_output(FORWARD,Mag_Advance,max_angle);
                *next_state = 10;

                printf("Present State: %d 1 FORWARD\n", state);
                break;

        case 10: // Forward
                gen_vector=generate_output(FORWARD,Mag_Advance,max_angle);
                *next_state = 0;

                printf("Present State: %d 2 FORWARD\n", state);
                break;

	case 11: // Right turn
                gen_vector=generate_output(RIGHT,Mag_Advance,max_angle);
                *next_state = 12;

                printf("Present State: %d turn 1 RIGHT\n", state);
                break;

        case 12: // Right turn
                gen_vector=generate_output(RIGHT,Mag_Advance,max_angle);
                *next_state = 0;

                printf("Present State: %d turn 2 RIGHT\n", state);
                break;


        case 13: // // check destination
		 if (dest == 0){
                                // go right
                                gen_vector=generate_output(RIGHT,Mag_Advance,max_angle);
                                *next_state = 5;

                                printf("Present State: %d RIGHT\n", state);
                 }
                 else if (dest == 1){
                                // go left
                                gen_vector=generate_output(LEFT,Mag_Advance,max_angle);
                                *next_state = 3;

                                printf("Present State: %d LEFT\n", state);
                 }
                 else if (dest == 2){
                                // go right single
                                gen_vector=generate_output(FORWARD,Mag_Advance,max_angle);
                                *next_state = 5;

                                printf("Present State: %d FORWARD\n", state);
                 }
                 else if (dest == 3){
                                // go left single
                                gen_vector=generate_output(FORWARD,Mag_Advance,max_angle);
                                *next_state = 3;

                                printf("Present State: %d FORWARD\n", state);
                 }
                break;

	default:
		printf("State %d not defined used ", state);
                gen_vector=generate_output(STOP,Mag_Advance,max_angle);
                next_state = 0;
                break;

                
 }

 return gen_vector;

}

// Student BUG 1
AdvanceAngle bug1_students(Raw observations, int dest, coord coord_robot, int intensity, int state, int *next_state, float Mag_Advance, float max_angle, int num_sensors){

 AdvanceAngle gen_vector;
 int obs;
 int a; //variable por si encuentra un obstaculo
 int j;
 float left_side=0;
 float right_side=0;
 int value = 0;
 coord coord_obs;
 coord coord_per;
 //float x =coord_robot.xc;
 //float y =coord_robot.yc;
 printf("\n\n **************** Student State Machine BUG I *********************\n");

 for(j=0;j<num_sensors/2;j++){
        right_side = observations.sensors[j] + right_side;
        printf("right side sensor[%d] %f\n",j,observations.sensors[j]);
 }

 for(j=num_sensors/2;j<num_sensors;j++){
        left_side = observations.sensors[j] + left_side;
        printf("left side sensor[%d] %f\n",j,observations.sensors[j]);
 }

 right_side = right_side/(num_sensors/2);
 left_side = left_side/(num_sensors/2);
 printf("Average right side %f\n",right_side);
 printf("Average left side %f\n",left_side);

 if( left_side < THRS) value = (value << 1) + 1;
 else value = (value << 1) + 0;

 if( right_side < THRS) value = (value << 1) + 1;
 else value = (value << 1) + 0;
 
 if(a!=1) a=0;
 obs = value;
 printf("intensity %d obstacles %d dest %d\n",intensity,obs,dest);

 switch ( state ) {

        case 0:
                if (intensity == 1){
                        gen_vector=generate_output(STOP,Mag_Advance,max_angle);
                        *next_state = 1;

                        printf("Present State: %d STOP\n", state);
			printf("\n **************** Reached light source ******************************\n");
                }
                else{

			gen_vector=generate_output(FORWARD,Mag_Advance,max_angle);
                        *next_state = 1;

                        printf("Present State: %d FORWARD\n", state);
                }

                break;

        case 1:
                if (obs == 0 && a == 0){
			// There is not obstacle
                        gen_vector=generate_output(FORWARD,Mag_Advance,max_angle);
                        *next_state = 20;

                        printf("Present State: %d FORWARD\n", state);
                }
                else{
			a=1;
                        gen_vector=generate_output(STOP,Mag_Advance,max_angle);
                        printf("Present State: %d STOP\n", state);

                        if (obs == 1){
                                // obtacle in the  right
                                *next_state = 7;
                        }
                        else if (obs == 2){
                                // obtacle in the left
                                *next_state = 4;
                        }
                        else if (obs == 3){
				// obstacle in the front
                                *next_state = 2;
                        }
                        else if (obs == 0){
                                // obtacle lost
                                *next_state = 5;
                        }
                }

                break;

        case 2: // Backward, obstacle in the front
                gen_vector=generate_output(BACKWARD,Mag_Advance,max_angle);
                *next_state = 3;

		printf("Present State: %d BACKWARD, obstacle right\n", state);
                break;

        case 3: // left turn
                gen_vector=generate_output(LEFT,Mag_Advance,max_angle);
                *next_state = 6;

		printf("Present State: %d TURN LEFT\n", state);
                break;

        case 4: // Backward, obstacle in the left
                gen_vector=generate_output(BACKWARD,Mag_Advance,max_angle);
                *next_state = 3;

		printf("Present State: %d BACKWARD, obstacle left\n", state);
                break;

        case 5: // right turn
                gen_vector=generate_output(RIGHT,Mag_Advance,max_angle);
                *next_state = 9;

		printf("Present State: %d TURN RIGHT\n", state);
                break;

        case 6: // Backward, obstacle in front
		//coord_obs = coord_robot;
		//a=1;                
		gen_vector=generate_output(FORWARD,Mag_Advance,max_angle);
                *next_state = 1;

		printf("Present State: %d BACKWARD, obstacle FRONT\n", state);
                break;
        case 7: // Backward, obstacle in front
		coord_per = coord_robot;                
		gen_vector=generate_output(FORWARD,Mag_Advance,max_angle);
                *next_state = 8;

		printf("Present State: %d BACKWARD, obstacle FRONT\n", state);
                break;
        case 8: // Backward, obstacle in front
		if(coord_per.xc==coord_robot.xc){
                	gen_vector=generate_output(LEFT,Mag_Advance,max_angle);
                	*next_state = 6;
		}
		else{
			//gen_vector=generate_output(BACKWARD,Mag_Advance,max_angle);
                	*next_state = 6;
		}

		printf("Present State: %d BACKWARD, obstacle FRONT\n", state);
                break;

        case 9: // Backward, obstacle in front
		//coord_obs = coord_robot;                
		gen_vector=generate_output(RIGHT,Mag_Advance,max_angle);
                *next_state = 1;

		printf("Present State: %d BACKWARD, obstacle FRONT\n", state);
                break;

        case 10: // Backward, obstacle in front
		//coord_obs = coord_robot;                
		gen_vector=generate_output(FORWARD,Mag_Advance,max_angle);
                *next_state = 11;

		printf("Present State: %d BACKWARD, obstacle FRONT\n", state);
                break;
        case 11: // Backward, obstacle in front
		//coord_obs = coord_robot;                
		gen_vector=generate_output(RIGHT,Mag_Advance,max_angle);
                *next_state = 12;

		printf("Present State: %d BACKWARD, obstacle FRONT\n", state);
                break;
        case 12: // Backward, obstacle in front
		//coord_obs = coord_robot;                
		gen_vector=generate_output(RIGHT,Mag_Advance,max_angle);
                *next_state = 13;

		printf("Present State: %d BACKWARD, obstacle FRONT\n", state);
                break;
        case 13: // Backward, obstacle in front
		//coord_obs = coord_robot;                
		gen_vector=generate_output(FORWARD,Mag_Advance,max_angle);
                *next_state = 14;

		printf("Present State: %d BACKWARD, obstacle FRONT\n", state);
                break;
        case 14: // Backward, obstacle in front
		if(obs == 3){
                	*next_state = 7;
		}
		else if(obs == 1){
                                // obtacle in the right
			gen_vector=generate_output(LEFT,Mag_Advance,max_angle);
                        *next_state = 8;
                }
		else{
			*next_state = 9;
		}

		printf("Present State: %d BACKWARD, obstacle FRONT\n", state);
                break;
        case 15: // Backward, obstacle in front              
		gen_vector=generate_output(BACKWARD,Mag_Advance,max_angle);
                *next_state = 7;

		printf("Present State: %d BACKWARD, obstacle FRONT\n", state);
                break;

        case 18: // Backward, obstacle in front
		if(coord_robot.xc == coord_obs.xc && coord_robot.yc == coord_obs.yc){
			gen_vector=generate_output(BACKWARD,Mag_Advance,max_angle);
                	*next_state = 6;
		}
		else{
			*next_state = 6;
		}

		printf("Present State: %d BACKWARD, obstacle FRONT\n", state);
                break;
	

        case 20: // // check destination
		 if (dest == 0){
                                // go right
                                gen_vector=generate_output(RIGHT,Mag_Advance,max_angle);
                                *next_state = 5; //Right

                                printf("Present State: %d RIGHT\n", state);
                 }
                 else if (dest == 1){
                                // go left
                                gen_vector=generate_output(LEFT,Mag_Advance,max_angle);
                                *next_state = 3;

                                printf("Present State: %d LEFT\n", state);
                 }
                 else if (dest == 2){
                                // go right single
                                gen_vector=generate_output(FORWARD,Mag_Advance,max_angle);
                                *next_state = 5;

                                printf("Present State: %d FORWARD\n", state);
                 }
                 else if (dest == 3){
                                // go left single
                                gen_vector=generate_output(FORWARD,Mag_Advance,max_angle);
                                *next_state = 3;

                                printf("Present State: %d FORWARD\n", state);
                 }
                break;

	default:
		printf("State %d not defined used ", state);
                gen_vector=generate_output(STOP,Mag_Advance,max_angle);
                next_state = 0;
                break;

                
 }

 return gen_vector;

}


// Campos potenciales 1
AdvanceAngle Campos_P(Raw observations, float dest_angle, float distance, coord coord_robot, int intensity, int a, int *next_a, float xq, float *next_xq, float yq, float *next_yq, float Mag_Advance, float max_angle, int num_sensors, float angle_start, float angle_end){

 AdvanceAngle gen_vector;
 int obs;
 int j,k,l; //variable por si encuentra un obstaculo
 float d;
 float angulo;
 float left_side=0;
 float right_side=0;
 int value = 0;
 float e1 = 5;
 float x,xd,xo,y,yd,yo;
 int caso=5;
 float Fx,Fy,fx,fy, dobs, FRx,FRy, FAx,FAy,fax,fay,mod;
 float d0=5.0, n = 2, obst_angle=3.14;
 coord coord_obs;
 coord coord_per;
 //float x =coord_robot.xc;
 //float y =coord_robot.yc;
 
 d=distance;
 
 xd=d*cos(dest_angle); 
 yd=d*sin(dest_angle);

 if (a!=1) {

 	xq=0;
	yq=0;
	*next_a=1;
 
 }

 if (intensity == 1){
         gen_vector=generate_output(STOP,Mag_Advance,max_angle);
         printf("Present State: STOP\n");		
 }
 else{

 	 x=xq;
	 y=yq; 

	 printf("\n\n **************** Student reactive Campos *********************\n");

	 //U=(1/2)*e1*(((x-xd)*(x-xd))+((y-yd)*(y-yd)));

	 //F=e1*((x-xd),(y-yd));
	 FAx= e1*(x-xd);
	 FAy= e1*(y-yd);

	 mod = (sqrt(((FAx)*(FAx))+((FAy)*(FAy))));

	 fx=FAx/mod;
	 fy=FAy/mod;

	 printf("Fuerza de atraccion: %f , %f \n", FAx,FAy);
	 printf("Fuerza de atraccion 2: %f , %f \n", fx,fy);

	 *next_xq=x-(Mag_Advance*fx);
	 *next_yq=y-(Mag_Advance*fy);

	 xq=x-(Mag_Advance*fx);
	 yq=y-(Mag_Advance*fy);

	 printf("Posicion actual: %f , %f \n", x,y);
	 printf("Posicion siguiente: %f , %f \n", xq,yq);
	 printf("Posicion destino: %f , %f \n", xd,yd);

	 
	 angulo=atan(yq/xq);
	
	 printf("Angulos: %f \n", angulo);

	 if((xq<0 && yq<0) | xq<0){

	 	angulo=3.1415+atan(yq/xq);		
		
	 }

	 gen_vector=generate_output(caso,Mag_Advance,angulo);

	 printf("Angulos: %f , %f \n", angulo, dest_angle);

 }


 return gen_vector;

}


// Campos potenciales 2
AdvanceAngle Campos_P2(Raw observations, float dest_angle, float distance, coord coord_robot, int intensity, int a, int *next_a, float xq, float *next_xq, float yq, float *next_yq, float Mag_Advance, float max_angle, int num_sensors, float angle_start, float angle_end){

 AdvanceAngle gen_vector;
 int obs;
 int j,l=0,m=0; //variable por si encuentra un obstaculo
 float d;
 float angulo, angulo1;
 float left_side=0;
 float right_side=0;
 int value = 0;
 float e1 = 200;
 float x,xd,xo,y,yd,yo,xq1,yq1;
 int caso=5;
 float Fx,Fy,fx,fy,fx1,fy1, dobs, FRx,FRy, FAx,FAy;
 float d0=5.0, n = 0.08, obst_angle=3.14, mod;
 float r1,r2,r3,r4,modr,ks,ns,k1;
 coord coord_obs;
 coord coord_per;
 //float x =coord_robot.xc;
 //float y =coord_robot.yc;
 
 d=distance;

 //dobs=10;

 k1=(abs(angle_start)+abs(angle_end));
 ns= num_sensors;
 ks=k1/ns;
 printf("k1 k: %f, %f \n",k1, ks);


 for(j=0;j<num_sensors;j++){

	if(observations.sensors[j] < 0.1){
		dobs=observations.sensors[j];
		obst_angle=angle_start+(j*ks);
		l=1;
		printf("Angulos1: %f, %f \n", obst_angle, angle_start);
		printf("j k: %d, %f \n", j, ks);
		xo=dobs*cos(obst_angle); 
		yo=dobs*sin(obst_angle); 
		
	}
	else if(l!=1){
		printf("Anguloss: %f \n", obst_angle);
		xo=0; 
		yo=0; 

	}	
 }

 

 xd=d*cos(dest_angle); 
 yd=d*sin(dest_angle);

 printf("xobs, yobs: %f, %f \n", xo, yo);

 if (a!=1) {

 	xq=0;
	yq=0;
	*next_a=1;
 
 }

 if (intensity == 1){
         gen_vector=generate_output(STOP,Mag_Advance,max_angle);
         printf("Present State: STOP\n");		
 }
 else{

 	 x=xq;
	 y=yq; 

	 printf("\n\n **************** Student reactive Campos *********************\n");

	 //U=(1/2)*e1*(((x-xd)*(x-xd))+((y-yd)*(y-yd)));

	 modr= sqrt(((-xo)*(-xo))+((-yo)*(-yo)));

	 r1 = ((1/modr)-(1/d0));
	 r2 = (1/(modr*modr));
	 r3 = ((-xo)/modr);
	 r4 = ((-yo)/modr);

	 printf("rs: %f, %f, %f, %f \n", r1,r2,r3,r4);

	 FRx=(-n)*r1*r2*r3;
	 FRy=(-n)*r1*r2*r4;

	 if(modr>d0 | (xo==0 && l!=1)){

	 	FRx=0;
		FRy=0;
	 }
	 else if(xo==0){

		m=1;

	 }

	 printf("Fuerza de repulsion: %f , %f \n", FRx,FRy);

	 //F=e1*((x-xd),(y-yd));
	 FAx= e1*(x-xd);
	 FAy= e1*(y-yd);

	 Fx= FRx+FAx;
	 Fy= FRy+FAy;

	 mod = (sqrt(((Fx)*(Fx))+((Fy)*(Fy))));

	 fx=Fx/mod;
	 fy=Fy/mod;

	 printf("Fuerza de atraccion: %f , %f \n", FAx,FAy);
	 printf("Fuerza total: %f , %f \n", Fx,Fy);
	 printf("Fuerza total m: %f , %f, %f \n", fx,fy, mod);

	 *next_xq=x-(Mag_Advance*fx);
	 *next_yq=y-(Mag_Advance*fy);

	 xq=x-(Mag_Advance*fx);
	 yq=y-(Mag_Advance*fy);

	 printf("Posicion siguiente: %f , %f \n", xq,yq);
	 
	 angulo=atan(yq/xq);

	 if((xq<0 && yq<0 && FRx==0) | (xq<0 && FRx==0)){

	 	angulo=3.1415+atan(yq/xq);
		printf("KKKKKKKKKKK \n");		
		
	 }
	 if(xo<0){

	 	angulo=3.1415+atan(yq/xq);
		printf("KKKKKKKKKKK \n");	

	 }
	 if(m==1){

	 	angulo=3.1415;
		printf("MMMMMMMMMMMM \n");

	 }

	 printf("Angulos: %f, %f \n", angulo, dest_angle);

	 if(FRx==0){
		gen_vector=generate_output(caso,Mag_Advance,angulo);
		printf("Holas \n");
	 }
	 else{
	 	gen_vector=generate_output(caso,Mag_Advance,angulo);
	 }
 }


 return gen_vector;

}
                 


// Campos potenciales con coordenadas
AdvanceAngle Campos_C(Raw observations, float dest_angle, float distance, coord coord_robot, int intensity, int a, int *next_a, float xq, float *next_xq, float yq, float *next_yq, float Mag_Advance, float max_angle, int num_sensors, float angle_start, float angle_end){

 AdvanceAngle gen_vector;
 int obs;
 int j,k,l; //variable por si encuentra un obstaculo
 float d;
 float angulo;
 float left_side=0;
 float right_side=0;
 int value = 0;
 float e1 = 5;
 float xd,xo,yd,yo;
 int caso=5;
 float Fx,Fy,fx,fy, dobs, FRx,FRy, FAx,FAy,fax,fay;
 float d0=5.0, n = 2, obst_angle=3.14;
 coord coord_obs;
 coord coord_per;
 float x = coord_robot.xc;
 float y = coord_robot.yc;
 
 d = distance;

 xd=d*cos(dest_angle); 
 yd=d*sin(dest_angle); 


 if (intensity == 1){
         gen_vector=generate_output(STOP,Mag_Advance,max_angle);
         printf("Present State: STOP\n");		
 }
 else{

	 printf("\n\n **************** Student reactive Campos *********************\n");

	 //U=(1/2)*e1*(((x-xd)*(x-xd))+((y-yd)*(y-yd)));

	 //F=e1*((x-xd),(y-yd));
	 FAx= e1*(x-xd);
	 FAy= e1*(y-yd);

	 fax=FAx/(sqrt(((x-xd)*(x-xd))+((y-yd)*(y-yd))));
	 fay=FAy/(sqrt(((x-xd)*(x-xd))+((y-yd)*(y-yd))));

	 //Fx= FRx+FAx;
	 //Fy= FRy+FAy;

	 fx=FAx/(sqrt(((FAx)*(FAx))+((FAy)*(FAy))));
	 fy=FAy/(sqrt(((FAx)*(FAx))+((FAy)*(FAy))));

	 printf("x, y destino: %f , %f \n", xd,yd);
	 printf("Fuerza de atraccion: %f , %f \n", FAx,FAy);
	 printf("Fuerza de atraccion: %f , %f \n", fax,fay);
	 printf("Fuerza de atraccion: %f , %f \n", fx,fy);

	 *next_xq=x-(Mag_Advance*fx);
	 *next_yq=y-(Mag_Advance*fy);

	 xq=x-(Mag_Advance*fx);
	 yq=y-(Mag_Advance*fy);

	 printf("Posicion siguiente: %f , %f \n", xq,yq);
	 
	 angulo=atan(fy/fx);

	 gen_vector=generate_output(caso,Mag_Advance,angulo);

	 printf("Angulos: %f , %f \n", angulo, dest_angle);

 }


 return gen_vector;

}


// Campos potenciales 2
AdvanceAngle Campos_C2(Raw observations, float dest_angle, coord coord_robot, int intensity, int a, int *next_a, float xq, float *next_xq, float yq, float *next_yq, float Mag_Advance, float max_angle, int num_sensors, float angle_start, float angle_end){

 AdvanceAngle gen_vector;
 int obs;
 int j,k,l=0; //variable por si encuentra un obstaculo
 int d=1;
 float angulo;
 float left_side=0;
 float right_side=0;
 int value = 0;
 int e1 = 3;
 float x,xd,xo,y,yd,yo;
 int caso=5;
 float Fx,Fy,fx,fy, dobs, FRx,FRy, FAx,FAy;
 float d0=5.0, n = 2, obst_angle=3.14;
 coord coord_obs;
 coord coord_per;
 //float x =coord_robot.xc;
 //float y =coord_robot.yc;
 
 

 dobs=10;

 k=(abs(angle_start-angle_end))/num_sensors;

 for(j=0;j<num_sensors;j++){

	if(observations.sensors[j] < 0.1 && observations.sensors[j]<dobs){
		dobs=observations.sensors[j];
		obst_angle=angle_start+(j*k);
		l=1;
		printf("Anguloss: %f \n", observations.sensors[j]);
		
	}
	else if(observations.sensors[j]==0.100000 && l!=1){

		dobs=0;

	}	
 }

 

 xd=d*sin(dest_angle); 
 yd=d*cos(dest_angle);

 xo=dobs*sin(obst_angle); 
 yo=dobs*cos(obst_angle); 

 if (a!=1) {

 	xq=0;
	yq=0;
	*next_a=1;
 
 }

 if (intensity == 1){
         gen_vector=generate_output(STOP,Mag_Advance,max_angle);
         printf("Present State: STOP\n");		
 }
 else{

 	 x=xq;
	 y=yq; 

	 printf("\n\n **************** Student reactive Campos *********************\n");

	 //U=(1/2)*e1*(((x-xd)*(x-xd))+((y-yd)*(y-yd)));

	 FRx=(-n)*((1/(sqrt(((x-xo)*(x-xo))+((y-yo)*(y-yo)))))-(1/d0))*(1/(((x-xo)*(x-xo))+((y-yo)*(y-yo))))*((x-xo)/(sqrt(((x-xo)*(x-xo))+((y-yo)*(y-yo)))));
	 FRy=(-n)*((1/(sqrt(((x-xo)*(x-xo))+((y-yo)*(y-yo)))))-(1/d0))*(1/(((x-xo)*(x-xo))+((y-yo)*(y-yo))))*((y-yo)/(sqrt(((x-xo)*(x-xo))+((y-yo)*(y-yo)))));

	 if(((sqrt(((x-xo)*(x-xo))+((y-yo)*(y-yo))))>d0) | dobs==0){

	 	FRx=0;
		FRy=0;
	 }

	 printf("Fuerza de repulsion: %f , %f \n", FRx,FRy);

	 //F=e1*((x-xd),(y-yd));
	 FAx= e1*(x-xd);
	 FAy= e1*(y-yd);

	 //fax=FAx/(sqrt(((x-xd)*(x-xd))+((y-yd)*(y-yd))));
	 //fay=FAy/(sqrt(((x-xd)*(x-xd))+((y-yd)*(y-yd))));

	 Fx= FRx+FAx;
	 Fy= FRy+FAy;

	 fx=Fx/(sqrt(((Fx)*(Fx))+((Fy)*(Fy))));
	 fy=Fy/(sqrt(((Fx)*(Fx))+((Fy)*(Fy))));

	 printf("Fuerza de atraccion: %f , %f \n", FAx,FAy);

	 *next_xq=x-(Mag_Advance*fx);
	 *next_yq=y-(Mag_Advance*fy);

	 xq=x-(Mag_Advance*fx);
	 yq=y-(Mag_Advance*fy);

	 printf("Posicion siguiente: %f , %f \n", xq,yq);
	 
	 angulo=atan(yq/xq);

	 printf("Angulo: %f \n", angulo);
	 printf("Angulo_dest: %f \n", dest_angle);

	 if(FRx==0){
		gen_vector=generate_output(caso,Mag_Advance,dest_angle);
	 }
	 else{
	 	gen_vector=generate_output(caso,Mag_Advance,angulo);
	 }
 }


 return gen_vector;

}



