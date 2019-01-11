  /*
   *  cpu.c
   *  Contains APEX cpu pipeline implementation
   *
   *  Author :
   *  Gaurav Kothari (gkothar1@binghamton.edu)
   *  State University of New York, Binghamton
   */
  #include <stdio.h> 
  #include <stdlib.h> 
  #include <string.h>

  #include "cpu.h"

  /* Set this flag to 1 to enable debug messages */
  #define ENABLE_DEBUG_MESSAGES 1

  /*
   * This function creates and initializes APEX cpu.
   *
   * Note : You are free to edit this function according to your
   *        implementation
   */
    int sum=2;
  APEX_CPU *
   APEX_cpu_init(const char * filename) {
    if (!filename) {
     return NULL;
    }

    APEX_CPU * cpu = malloc(sizeof( * cpu));
    if (!cpu) {
     return NULL;
    }

    /* Initialize PC, Registers and all pipeline stages */
    cpu -> pc = 4000;
    memset(cpu -> regs, 0, sizeof(int) * 32);
    memset(cpu -> regs_valid, 1, sizeof(int) * 32);
    memset(cpu -> stage, 0, sizeof(CPU_Stage) * NUM_STAGES);
    memset(cpu -> data_memory, 0, sizeof(int) * 4000);

    /* Parse input file and create code memory */
    cpu -> code_memory = create_code_memory(filename, & cpu -> code_memory_size);

    if (!cpu -> code_memory) {
     free(cpu);
     return NULL;
    }

    if (ENABLE_DEBUG_MESSAGES) {
     fprintf(stderr,
      "APEX_CPU : Initialized APEX CPU, loaded %d instructions\n",
      cpu -> code_memory_size);
     fprintf(stderr, "APEX_CPU : Printing Code Memory\n");
     printf("%-9s %-9s %-9s %-9s %-9s\n", "opcode", "rd", "rs1", "rs2", "imm");

     for (int i = 0; i < cpu -> code_memory_size; ++i) {
      printf("%-9s %-9d %-9d %-9d %-9d\n",
       cpu -> code_memory[i].opcode,
       cpu -> code_memory[i].rd,
       cpu -> code_memory[i].rs1,
       cpu -> code_memory[i].rs2,
       cpu -> code_memory[i].imm);
     }
    }

    /* Make all stages busy except Fetch stage, initally to start the pipeline */
    for (int i = 1; i < NUM_STAGES; ++i) {
     cpu -> stage[i].busy = 1;
    }

    return cpu;
   }

  /*
   * This function de-allocates APEX cpu.
   *
   * Note : You are free to edit this function according to your
   *        implementation
   */
  void
  APEX_cpu_stop(APEX_CPU * cpu) {
   free(cpu -> code_memory);
   free(cpu);
  }

  /* Converts the PC(4000 series) into
   * array index for code memory
   *
   * Note : You are not supposed to edit this function
   *
  ` */
  int
  get_code_index(int pc) {
   return (pc - 4000) / 4;
  }

  static void
  print_instruction(CPU_Stage * stage) {
   if (strcmp(stage -> opcode, "STORE") == 0) {
    printf("%s,R%d,R%d,#%d ", stage -> opcode, stage -> rs1, stage -> rs2, stage -> imm);
   }

   if (strcmp(stage -> opcode, "LDR") == 0) {
    printf("%s,R%d,R%d,R%d", stage -> opcode, stage -> rd, stage -> rs1, stage -> rs2);
   }

   if (strcmp(stage -> opcode, "MOVC") == 0) {
    printf("%s,R%d,#%d ", stage -> opcode, stage -> rd, stage -> imm);
   }

   if (strcmp(stage -> opcode, "ADD") == 0) {
    printf("%s,R%d,R%d,R%d", stage -> opcode, stage -> rd, stage -> rs1, stage -> rs2);
   }

   if (strcmp(stage -> opcode, "SUB") == 0) {
    printf("%s,R%d,R%d,R%d", stage -> opcode, stage -> rd, stage -> rs1, stage -> rs2);
   }

   if (strcmp(stage -> opcode, "AND") == 0) {
    printf("%s,R%d,R%d,R%d", stage -> opcode, stage -> rd, stage -> rs1, stage -> rs2);
   }

   if (strcmp(stage -> opcode, "OR") == 0) {
    printf("%s,R%d,R%d,R%d", stage -> opcode, stage -> rd, stage -> rs1, stage -> rs2);
   }

   if (strcmp(stage -> opcode, "XOR") == 0) {
    printf("%s,R%d,R%d,R%d ", stage -> opcode, stage -> rd, stage -> rs1, stage -> rs2);
   }



   if (strcmp(stage -> opcode, "LOAD") == 0) {
    printf("%s,R%d,R%d,#%d ", stage -> opcode, stage -> rd, stage -> rs1, stage -> imm);
   }

  if (strcmp(stage -> opcode, "MUL") == 0) {
    printf("%s,R%d,R%d,R%d ", stage -> opcode, stage -> rd, stage -> rs1, stage -> rs2);
   }

  if (strcmp(stage -> opcode, "NOP") == 0) {
    printf("%s ", stage -> opcode);
   }

  if (strcmp(stage -> opcode, "HALT") == 0) {
    printf("%s ", stage -> opcode);
   }
  if (strcmp(stage -> opcode, "BZ") == 0) {
    printf("%s,#%d ", stage -> opcode, stage -> imm);
   }
  if (strcmp(stage -> opcode, "BNZ") == 0) {
    printf("%s,#%d ", stage -> opcode,stage -> imm);
   }

  if (strcmp(stage -> opcode, "JUMP") == 0) {
    printf("%s,R%d,#%d", stage -> opcode, stage -> rs1, stage ->imm);
   }



  }

  /* Debug function which dumps the cpu stage
   * content
   *
   * Note : You are not supposed to edit this function
   *
   */
  static void
  print_stage_content(char * name, CPU_Stage * stage) {
   printf("%-15s: pc(%d) ", name, stage -> pc);
   print_instruction(stage);
   printf("\n");
  }

  /*
   *  Fetch Stage of APEX Pipeline
   *
   *  Note : You are free to edit this function according to your
   *         implementation
   */
int fetch(APEX_CPU * cpu) {
  CPU_Stage * stage = &(cpu->stage[F]);
  if (!stage->busy && !stage->stalled  ) {
    /* Store current PC in fetch latch */
    stage->pc = cpu->pc;
    /* Index into code memory using this pc and copy all instruction fields into
     * fetch latch
     */
    APEX_Instruction * current_ins = & cpu -> code_memory[get_code_index(cpu -> pc)];
    if(cpu->r==1)
    {
      strcpy(stage -> opcode,"NOP");
      cpu->pc= cpu->sum;
      cpu->r=0;
    }else{
      strcpy(stage -> opcode, current_ins -> opcode);
    }
    stage -> rd = current_ins -> rd;
    stage -> rs1 = current_ins -> rs1;
    stage -> rs2 = current_ins -> rs2;
    stage -> imm = current_ins -> imm;
    stage -> rd = current_ins -> rd;
    /* Update PC for next instruction */
    cpu -> pc += 4;
    /* Copy data from fetch latch to decode latch*/
    if( (&cpu -> stage[DRF])->stalled != 1)
    {
      cpu -> stage[DRF] = cpu -> stage[F];
      (&cpu -> stage[F])->stalled = 0;
    }
    else
    { 
    (&cpu -> stage[F])->stalled = 1;
    }
    if ((strcmp((cpu->func),"display")==0)) {
      print_stage_content("Fetch", stage);
    }
  }
  else if ((&cpu -> stage[DRF])->stalled != 1 && (&cpu -> stage[F])->stalled != 1 && ENABLE_DEBUG_MESSAGES) {
    print_stage_content("Fetch", stage);
  }
  else if(( &cpu -> stage[DRF])->stalled != 1 && (&cpu -> stage[F])->stalled== 1)
  {
    (&cpu -> stage[F])->stalled = 0;
    print_stage_content("Fetch", stage);
    cpu -> stage[DRF] = cpu -> stage[F];
  }
   else {print_stage_content("Fetch", stage);}
   return 0;
  }

  /*
   *  Decode Stage of APEX Pipeline
   *
   *  Note : You are free to edit this function according to your
   *         implementation
   */
  int
  decode(APEX_CPU * cpu) {
   CPU_Stage * stage = & cpu -> stage[DRF];
	if(strcmp(stage->opcode, "BNZ") == 0 || strcmp(stage->opcode, "BZ") == 0) {
     if(strcmp((&cpu->stage[WB])->opcode, "ADD") != 0 && strcmp((&cpu->stage[WB])->opcode, "SUB") != 0 && strcmp((&cpu->stage[WB])->opcode, "MUL") != 0
&& strcmp((&cpu->stage[MEM])->opcode, "ADD") != 0 && strcmp((&cpu->stage[MEM])->opcode, "SUB") != 0 && strcmp((&cpu->stage[MEM])->opcode, "MUL") != 0) {
         stage->stalled = 0;
         ( & cpu -> stage[DRF])->stalled = 0;
        }
   }
/*JUMP*/
 if (strcmp(stage -> opcode, "JUMP") == 0) {
     if ((cpu -> regs_valid[stage -> rs1])) {
      stage -> stalled = 0;

     } 
}

  if(strcmp(stage -> opcode, "ADD") == 0 || strcmp(stage -> opcode, "SUB") == 0) {

   if (cpu -> regs_valid[stage -> rs1] && cpu -> regs_valid[stage -> rs2]) {
    stage -> stalled = 0;
    //(&cpu->stage[F])->stalled = 0;
     
   }
  }

  if(strcmp(stage -> opcode, "AND") == 0 || strcmp(stage -> opcode, "OR") == 0 || strcmp(stage -> opcode, "LDR") == 0) {
   if (cpu -> regs_valid[stage -> rs1] && cpu -> regs_valid[stage -> rs2]) {
    stage -> stalled = 0;
   // (&cpu->stage[F])->stalled = 0;

   }
  }

  if(strcmp(stage -> opcode, "XOR") == 0 || strcmp(stage -> opcode, "MUL") == 0 ) {
   if (cpu -> regs_valid[stage -> rs1] && cpu -> regs_valid[stage -> rs2]) {
    stage -> stalled = 0;
   // (&cpu->stage[F])->stalled = 0;

   }
  }

  if(strcmp(stage -> opcode, "STORE") == 0 ) {
    if (cpu -> regs_valid[stage -> rs1] && cpu -> regs_valid[stage -> rs2]) {
    stage -> stalled = 0;
   // (&cpu->stage[F])->stalled = 0;

   }
  }
 if(strcmp(stage -> opcode, "LOAD") == 0 ) {
    if (cpu -> regs_valid[stage -> rs1]) {
    stage -> stalled = 0;
   // (&cpu->stage[F])->stalled = 0;

   }
  }


   if (!stage -> busy && !stage -> stalled && (&cpu -> stage[EX])->stalled != 1) {

    if(strcmp(stage->opcode, "BNZ") == 0 || strcmp(stage->opcode, "BZ") == 0) {
     if(strcmp((&cpu->stage[WB])->opcode, "ADD") == 0 || strcmp((&cpu->stage[WB])->opcode, "SUB") == 0 || strcmp((&cpu->stage[WB])->opcode, "MUL") == 0
|| strcmp((&cpu->stage[MEM])->opcode, "ADD") == 0 || strcmp((&cpu->stage[MEM])->opcode, "SUB") == 0 || strcmp((&cpu->stage[MEM])->opcode, "MUL") == 0) {
         stage->stalled = 1;
       ( & cpu -> stage[DRF])->stalled = 1;
        }
   }
    /* STORE */
    if (strcmp(stage -> opcode, "STORE") == 0) {
     if ((cpu -> regs_valid[stage -> rs1] == 1) && (cpu -> regs_valid[stage-> rs2] == 1)) {
      stage -> rs1_value = cpu -> regs[stage -> rs1];
      stage -> rs2_value = cpu -> regs[stage -> rs2];
      //cpu -> regs_valid[stage -> rs1] = 0;

     } else {
      (&cpu -> stage[DRF])->stalled = 1;
      //(&cpu -> stage[F])->stalled = 1;


     }
    }

    /*  MOVC */
    if (strcmp(stage -> opcode, "MOVC") == 0) {
     cpu -> regs_valid[stage -> rd] = 0;
    }

    /*ADD and SUB and LDR*/

    if (strcmp(stage -> opcode, "ADD") == 0 || strcmp(stage -> opcode, "SUB") == 0 || strcmp(stage -> opcode, "LDR") == 0) {
     if ((cpu -> regs_valid[stage -> rs1]) && (cpu -> regs_valid[stage -> rs2])) {
      stage -> rs1_value = cpu -> regs[stage -> rs1];
      stage -> rs2_value = cpu -> regs[stage -> rs2];
      cpu -> regs_valid[stage -> rd] = 0;
     } else {

     (&cpu -> stage[DRF])->stalled = 1;
     //(&cpu -> stage[F])->stalled = 1;



     }
    }

/*AND  and  OR*/

    if (strcmp(stage -> opcode, "AND") == 0 || strcmp(stage -> opcode, "OR") == 0 || strcmp(stage -> opcode, "XOR") == 0) {
     if ((cpu -> regs_valid[stage -> rs1]) && (cpu -> regs_valid[stage -> rs2])) {
      stage -> rs1_value = cpu -> regs[stage -> rs1];
      stage -> rs2_value = cpu -> regs[stage -> rs2];
      cpu -> regs_valid[stage -> rd] = 0;
     } else {

     (&cpu -> stage[DRF])->stalled = 1;
     //(&cpu -> stage[F])->stalled = 1;



     }
    }



/*LOAD*/

    if (strcmp(stage -> opcode, "LOAD") == 0) {
     if ((cpu -> regs_valid[stage -> rs1])) {
      stage -> rs1_value = cpu -> regs[stage -> rs1];
      cpu -> regs_valid[stage -> rd] = 0;
     } else {

     (&cpu -> stage[DRF])->stalled = 1;
     //(&cpu -> stage[F])->stalled = 1;
       }
    }


  /*MUL*/

    if (strcmp(stage -> opcode, "MUL") == 0) {
     if ((cpu -> regs_valid[stage -> rs1] == 1) && (cpu -> regs_valid[stage -> rs2] == 1)) {
      stage -> rs1_value = cpu -> regs[stage -> rs1];
      stage -> rs2_value = cpu -> regs[stage -> rs2];
      cpu -> regs_valid[stage -> rd] = 0;
     } else {

     (&cpu -> stage[DRF])->stalled = 1;
    // (&cpu -> stage[F])->stalled = 1;


     }
    }

  /*NOP*/
  if (strcmp(stage -> opcode, "NOP") == 0) {
      printf("No action");
      }

  /*HALT*/
   if(strcmp(stage->opcode,"HALT")==0)
  {
  printf("No actions");
  }


  /*BZ*/
   if(strcmp(stage->opcode,"BZ")==0)
  {
  printf("No actions");
  }

  /*BNZ*/
   if(strcmp(stage->opcode,"BNZ")==0)
  {
  printf("No actions");
  }


  /*JUMP*/
   if (strcmp(stage -> opcode, "JUMP") == 0) {
     if ((cpu -> regs_valid[stage -> rs1])) {
      stage -> rs1_value = cpu -> regs[stage -> rs1];
     } 

else {

     (&cpu -> stage[DRF])->stalled = 1;

     }
    }



   

   /* Copy data from decode latch to execute latch*/
   if (!(&cpu->stage[DRF])->stalled) {
    cpu -> stage[EX] = cpu -> stage[DRF];
       (&cpu->stage[DRF])->stalled = 0;
   }
    

   if ((strcmp((cpu->func),"display")==0)) {
    print_stage_content("Decode/RF", stage);
   }
  

  else if ((strcmp((cpu->func),"display")==0)) {
  print_stage_content("Decode/RF", stage);
  }
  }
else if (strcmp((cpu->func),"display")==0) {
  print_stage_content("Decode/RF", stage);
 }

return 0;
 
  }
 



  /*
   *  Execute Stage of APEX Pipeline
   *
   *  Note : You are free to edit this function according to your
   *         implementation
   */
  int
  execute(APEX_CPU * cpu) {
    CPU_Stage * stage = & cpu -> stage[EX];
    if (!stage -> busy && !stage -> stalled  && (&cpu -> stage[MEM])->stalled != 1 ) {

     /* Store */
     if (strcmp(stage -> opcode, "STORE") == 0) {
      stage -> mem_address = (stage -> rs2_value) + (stage -> imm);
     }

     /* MOVC */
     if (strcmp(stage -> opcode, "MOVC") == 0) {
      stage -> buffer = (stage -> imm) + 0;
           }


     /*ADD */
     if (strcmp(stage -> opcode, "ADD") == 0) {
      stage -> buffer = (stage -> rs2_value) + (stage -> rs1_value);
     }
   int f=1;
  /*MUL */
     if (strcmp(stage -> opcode, "MUL") == 0) {
         stage->buffer=(stage->rs1_value) * (stage->rs2_value);
     // cpu -> eregs[stage -> rd] = (stage -> rs2_value) * (stage -> rs1_value);
    // cpu -> eregs_valid[stage -> rd] = 1;
     //while(sum>f)
   //  {
     (&cpu -> stage[DRF])->stalled = 1;
     (&cpu -> stage[F])->stalled = 1;
   (&cpu -> stage[EX])->stalled = 1;

      //  }

         }

     /* SUB */
     if (strcmp(stage -> opcode, "SUB") == 0) {
      stage -> buffer = (stage -> rs1_value) - (stage -> rs2_value);
     }

     /*AND*/
     if (strcmp(stage -> opcode, "AND") == 0) {
      stage -> buffer = (stage -> rs1_value) & (stage -> rs2_value);

     }


     /*LOAD */
     if (strcmp(stage -> opcode, "LOAD") == 0) {
      stage -> mem_address = (stage -> rs1_value) + (stage -> imm);
     }

     /*OR*/
     if (strcmp(stage -> opcode, "OR") == 0) {
      stage -> buffer = (stage -> rs1_value) | (stage -> rs2_value);
     }

     /*LDR*/
     if (strcmp(stage -> opcode, "LDR") == 0) {
      stage -> mem_address = (stage -> rs1_value) + (stage -> rs2_value);
     }

     /*XOR*/
     if (strcmp(stage -> opcode, "XOR") == 0) {
      stage -> buffer = (stage -> rs1_value) ^ (stage -> rs2_value);
     }
   

  /*BZ*/
     if (strcmp(stage -> opcode, "BZ") == 0) {
      if (strcmp((&cpu->stage[WB]) -> opcode, "ADD") == 0 || strcmp((&cpu->stage[WB])-> opcode, "SUB") == 0 || strcmp((&cpu->stage[WB])-> opcode, "MUL") == 0) {
  stage->stalled = 1;
    (&cpu -> stage[DRF])->stalled = 1;
     (&cpu -> stage[F])->stalled = 1;

}
else{
      if(cpu->flag==1)
       {
      cpu->sum=(stage->pc)+(stage->imm)-4;
       cpu->r=1;
           strcpy((&cpu->stage[DRF])->opcode,"NOP");
	cpu -> ins_completed = cpu -> sum/4;
      }
}
	
    //else
  //{
    // printf("BZ wont take any action");
    // }
  }

  /*BNZ*/
     if (strcmp(stage -> opcode, "BNZ") == 0) {
   if (strcmp((&cpu->stage[WB]) -> opcode, "ADD") == 0 || strcmp((&cpu->stage[WB])-> opcode, "SUB") == 0 || strcmp((&cpu->stage[WB])-> opcode, "MUL") == 0) {
  stage->stalled = 1;
    (&cpu -> stage[DRF])->stalled = 1;
     (&cpu -> stage[F])->stalled = 1;

}
else{
      if(cpu->flag==0)
       {
      cpu->sum=(stage->pc)+(stage->imm)-4;
      cpu->r=1;
       strcpy((&cpu->stage[DRF])->opcode,"NOP");
	cpu -> ins_completed = cpu -> sum/4;
      }
   }
  }


  /*HALT*/
   if(strcmp(stage->opcode,"HALT")==0){
     strcpy((&cpu->stage[DRF])->opcode,"NOP");
     strcpy((&cpu->stage[F])->opcode,"NOP");
     (&cpu -> stage[F])->busy = 1;
     (&cpu -> stage[DRF])->busy= 1;
      }

  /*JUMP*/
   if(strcmp(stage->opcode,"JUMP")==0){
     cpu->pc=(stage -> rs1_value) +(stage ->imm);
    strcpy((&cpu->stage[DRF])->opcode,"NOP");
     strcpy((&cpu->stage[EX])->opcode,"NOP");
	cpu -> ins_completed = cpu -> pc/4;

      }




     /* Copy data from Execute latch to Memory latch*/
     
       if( !(&cpu->stage[EX])->stalled){
      cpu -> stage[MEM] = cpu -> stage[EX];
  }
      
     if (strcmp((cpu->func),"display")==0) {
      print_stage_content("Execute", stage);
      

     }
    if ( !(&cpu->stage[EX])->stalled) {
     struct CPU_Stage bubble;
    cpu -> stage[EX] = bubble;
             }
     }
    else if (strcmp(stage->opcode,"MUL")==0 && stage->stalled) {
      cpu -> eregs[stage -> rd] = (stage -> rs2_value) * (stage -> rs1_value);
      cpu -> eregs_valid[stage -> rd] = 1;
     (&cpu -> stage[EX])->stalled = 0;

      (&cpu -> stage[DRF])->stalled = 0;
      (&cpu -> stage[F])->stalled = 0;
       cpu -> stage[MEM] = cpu -> stage[EX];
      if (strcmp((cpu->func),"display")==0) {
        print_stage_content("Execute", stage);

      }
 struct CPU_Stage bubble;
    cpu -> stage[EX] = bubble;


    }
else if(strcmp(stage->opcode,"BZ")==0 && stage->stalled) {
  stage->stalled = 0;
    (&cpu -> stage[DRF])->stalled = 0;
     (&cpu -> stage[F])->stalled = 0;
if(cpu->flag==1)
  {
      cpu->sum=(stage->pc)+(stage->imm)-4;
       cpu->r=1;
           strcpy((&cpu->stage[DRF])->opcode,"NOP");
	cpu -> ins_completed = cpu -> ins_completed + (stage->imm)/4;
      }
 if (strcmp((cpu->func),"display")==0) {
        print_stage_content("Execute", stage);

      }
    }
else if(strcmp(stage->opcode,"BNZ")==0 && stage->stalled) {
  stage->stalled = 0;
    (&cpu -> stage[DRF])->stalled = 0;
     (&cpu -> stage[F])->stalled = 0;
 if(cpu->flag==0)
  {
      cpu->sum=(stage->pc)+(stage->imm)-4;
       cpu->r=1;
           strcpy((&cpu->stage[DRF])->opcode,"NOP");
	cpu -> ins_completed = cpu -> ins_completed + (stage->imm)/4;
      }
 if (strcmp((cpu->func),"display")==0) {
        print_stage_content("Execute", stage);

      }

 struct CPU_Stage bubble;
    cpu -> stage[EX] = bubble;

}

     else if (strcmp((cpu->func),"display")==0) {
     print_stage_content("Execute", stage);

    }
   return 0;
    
  }
        /*
     *  Memory Stage of APEX Pipeline
     *
     *  Note : You are free to edit this function according to your
     *         implementation
     */
    int
    memory(APEX_CPU * cpu) {
     CPU_Stage * stage = & cpu -> stage[MEM];
      if (!stage -> busy && !stage -> stalled && (&cpu -> stage[WB])->stalled != 1)
       {
      /* Store */
      if (strcmp(stage -> opcode, "STORE") == 0) {
       cpu -> data_memory[stage -> mem_address] = stage -> rs1_value;

      }

      /* MOVC */
      if (strcmp(stage -> opcode, "MOVC") == 0) {

      }

      /*ADD*/
      if (strcmp(stage -> opcode, "ADD") == 0) {
       printf("Move forward");
      }

      /*SUB*/
      if (strcmp(stage -> opcode, "SUB") == 0) {
       printf("Move forward");
      }

      /*AND*/
      if (strcmp(stage -> opcode, "AND") == 0) {
       printf("Move forward");
      }

      /*OR*/
      if (strcmp(stage -> opcode, "OR") == 0) {
       printf("Move forward");
      }


      /*LOAD*/
      if (strcmp(stage -> opcode, "LOAD") == 0) {
       stage -> buffer = cpu -> data_memory[stage -> mem_address];
      }

      /*LDR*/
      if (strcmp(stage -> opcode, "LDR") == 0) {
       stage -> buffer = cpu -> data_memory[stage -> mem_address];
      }

      /*XOR*/
      if (strcmp(stage -> opcode, "XOR") == 0) {
       printf("Move forward");
      }
     /*HALT*/
       if (strcmp(stage -> opcode, "HALT") == 0) {
        
       }
     


      /* Copy data from memory latch to WB latch*/

     
       cpu -> stage[WB] = cpu -> stage[MEM];
 if (strcmp((cpu->func),"display")==0) {
       print_stage_content("Memory", stage);
       struct CPU_Stage bubble;
    cpu -> stage[MEM] = bubble;

      }
     

  }else if ( !(&cpu->stage[WB])->stalled) {
if (strcmp((cpu->func),"display")==0) {
     print_stage_content("Memory", stage);
     }

     struct CPU_Stage bubble;
    cpu -> stage[MEM] = bubble;
             }


   else if (strcmp((cpu->func),"display")==0) {
     print_stage_content("Memory", stage);
     }
     return 0;
     }
     
      /*
     *  Writeback Stage of APEX Pipeline
     *
     *  Note : You are free to edit this function according to your
     *         implementation
     */
    int
    writeback(APEX_CPU * cpu) {
     CPU_Stage * stage = & cpu -> stage[WB];
     if (!stage -> busy && !stage -> stalled ) {

      /*MOVC */
      if (strcmp(stage -> opcode, "MOVC") == 0) {
       cpu -> regs[stage -> rd] = stage -> buffer;
       cpu -> regs_valid[stage -> rd] = 1;

      }


      /*ADD*/
      if (strcmp(stage -> opcode, "ADD") == 0) {
       cpu -> regs[stage -> rd] = stage -> buffer;
       cpu -> regs_valid[stage -> rd] = 1;
        if(  cpu -> regs[stage -> rd]==0)
       {
         cpu->flag=1;
      }
     else
    {
     cpu->flag=0;
      }
      }
    

     /*MUL*/
      if (strcmp(stage -> opcode, "MUL") == 0) {
       cpu -> regs[stage -> rd] = stage -> buffer;
       cpu -> regs_valid[stage -> rd] = 1;
        if(  cpu -> regs[stage -> rd]==0)
       {
         cpu->flag=1;
      }
     else
    {
     cpu->flag=0;
      }

      }



      /*SUB*/
      if (strcmp(stage -> opcode, "SUB") == 0) {
       cpu -> regs[stage -> rd] = stage -> buffer;
       cpu -> regs_valid[stage -> rd] = 1;
        if(  cpu -> regs[stage -> rd]==0)
       {
         cpu->flag=1;
      }
     else
    {
     cpu->flag=0;
      }

      }

      /*AND*/
      if (strcmp(stage -> opcode, "AND") == 0) {
       cpu -> regs[stage -> rd] = stage -> buffer;

       cpu -> regs_valid[stage -> rd] = 1;
      }

   /*HALT*/
      if (strcmp(stage -> opcode, "HALT") == 0) {
          cpu->stop=1;
           }


      /*LOAD*/
      if (strcmp(stage -> opcode, "LOAD") == 0) {
       cpu -> regs[stage -> rd] = stage -> buffer;
       cpu -> regs_valid[stage -> rd] = 1;
      }


      /*OR*/
      if (strcmp(stage -> opcode, "OR") == 0) {
       cpu -> regs[stage -> rd] = stage -> buffer;
       cpu -> regs_valid[stage -> rd] = 1;
      }


      /*LDR*/
      if (strcmp(stage -> opcode, "LDR") == 0) {
       cpu -> regs[stage -> rd] = stage -> buffer;
       cpu -> regs_valid[stage -> rd] = 1;
      }

      /*XOR*/
      if (strcmp(stage -> opcode, "XOR") == 0) {
       cpu -> regs[stage -> rd] = stage -> buffer;
       cpu -> regs_valid[stage -> rd] = 1;
      }


      if(strcmp(stage -> opcode, "NOP") != 0 && stage ->pc >= 4000 && stage ->pc < 400000) {
        cpu->ins_completed++;
  }
      if (strcmp((cpu->func),"display")==0) {
       print_stage_content("Writeback", stage);
      }
    struct CPU_Stage bubble;
    cpu -> stage[WB] = bubble;
     }
     else if (strcmp((cpu->func),"display")==0) {
       print_stage_content("Writeback", stage);
      }

     return 0;
    }

    /*
     *  APEX CPU simulation loop
     *
     *  Note : You are free to edit this function according to your
     *         implementation
     */
    int
    APEX_cpu_run(APEX_CPU * cpu) {
 while (1) {

       printf("insCom : %d , code mem : %d",cpu -> ins_completed,cpu -> code_memory_size);
      /* All the instructions committed, so exit */
      if (cpu -> ins_completed == cpu -> code_memory_size || cpu->stop || cpu->clock==cpu->cycleNumber) {
       
       printf("(apex) >> Simulation Complete");
       break;
      }

      if (strcmp((cpu->func),"display")==0) {
       printf("--------------------------------\n");
       printf("Clock Cycle #: %d\n", cpu -> clock);
       printf("--------------------------------\n");
      }

      writeback(cpu);
      memory(cpu);
      execute(cpu);
      decode(cpu);
      fetch(cpu);
      cpu->clock++;
     }
printf("\n");
printf("==================REGISTER VALUE==============");
  for(int i=0;i<16;i++)
  {printf("\n");
  printf(" | Register[%d] | Value=%d | status=%s |",i,cpu->regs[i],(cpu->regs_valid[i])?"Valid" : "Invalid");
      
  }
printf("\n");
printf("==================DATA MEMORY ==============");
 printf("\n");
  for(int i=0;i<99;i++)
  {
  printf(" | MEM[%d] | Value=%d | \n",i,cpu->data_memory[i]);
  }
  return 0;
  }


     