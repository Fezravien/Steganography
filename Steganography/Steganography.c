//
//  Steganography.c
//
//
//  Created by 윤재웅 on 2020/07/29.
//  Copyright © 2020 pazravien. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

typedef unsigned char u8;

int main(){
   int pick;
   int pick1, pick2, pick3, pick4;
   int diff_state1, diff_state2;
   char diff_a, diff_b;
   int diff_count = 0;
   FILE *en_fp;
   FILE *en2_fp;
   FILE *de2_fp;
   FILE *en3_fp = NULL;
   FILE* diff_fp1;
   FILE* diff_fp2;
   //BMP 선언하는 부분(사전처리)
   int input_bit, h = 0, dec_inputlength = 0;
   int inputlengtharr[8],data[256],dec_data[256]; //최대 32글자만 입력한다 침
   u8 temp2 = 0;
   u8 input[32], dec_origindata[32];
   u8 header = 0, buffer1 = 0, buffer2 = 0, temp = 0;




one:printf("=========================================================\n");
   printf("1. JPG 파일의 EOI 이후에 사용자 입력하는 텍스트 정보 삽입/복원하기.\n");
   printf("2. JPG 파일의 헤더 이후에 사용자가 입력하는 텍스트 정보 삽입/복원하기.\n");
   printf("3. BMP 파일의 LSB를 변조하여 입력하는 텍스트 정보를 삽입/복원하기.\n");
   printf("4. 두 개의 파일을 입력으로 받아 서로 상이한 부분을 출력하기\n");
   printf("5. 종료\n");
   printf("=========================================================\n\n");

   printf("원하는 번호 입력(1~5) : ");
   scanf("%d", &pick);

two: while (1) {
       switch (pick) {

           case 1:

               printf("\n1. 삽입");
               printf("  2. 복구");
               printf("  3. 이전페이지 : ");

               scanf("%d", &pick1);

            while (1) {
                   switch (pick1) {
                       case 1: //삽입
                           en_fp = fopen("3.jpg", "ab");


                           char ec;
                           printf("input : ");
                           getchar();

                           while((ec=getchar()) != '\n'){
                               fprintf(en_fp, "%c", ec);
                           }

                           fclose(en_fp);

                           printf("삽입되었습니다.\n");


                           goto two;

                   case 2: //복구
                       system("xxd -p 3.jpg > hex.txt");

                       FILE *de_fp = fopen("hex.txt", "r");

                       char dc[3];
                       char buf[256];
                       int i=1, num1;
                       char temp[3];

                       while(1){
                           fseek(de_fp,-sizeof(char),SEEK_END);
                           fseek(de_fp, -i*sizeof(char)*2,SEEK_CUR);
                           fgets(dc,sizeof(dc),de_fp);
                           if(!strcmp(dc, "d9")) break;
                           i++;
                       }

                       fgets(buf, 256, de_fp);
                       temp[2] = '\0';

                       printf("result = ");

                       for(int i=0; i<strlen(buf)-1; i+=2){
                           temp[0] = buf[i];
                           temp[1] = buf[i+1];
                           num1 = (int)strtol(temp,NULL,16);
                           printf("%c",num1);
                       }

                       printf("\n복구했습니다.");
                       fclose(de_fp);

                           goto two;

                       case 3:
                       printf("이전페이지로 이동합니다.\n\n");
                           system("clear");
                           goto one;

                   default:
                       printf("잘못된 입력입니다.\n");
                           system("clear");
                           goto two;
               }
               }


               break;

           case 2:
               printf("\n1. 삽입");
               printf("  2. 복구");
               printf("  3. 이전페이지 : ");
               scanf("%d", &pick2);
               while (1) {

                   switch (pick2) {
                       case 1: //삽입
                           en2_fp = fopen("3.jpg", "r+b");

                           fseek(en2_fp, 10*sizeof(int),SEEK_SET);

                           char str[256];
                           printf("input : ");
                           scanf("%s",str);

                           char c='\0';
                           fprintf(en2_fp, "%s", str);
                           fprintf(en2_fp, "%c", c);

                           fclose(en2_fp);
                           goto two;

                       case 2: //복구

                       de2_fp = fopen("image.jpg", "r+b");

                       fseek(de2_fp, 10*sizeof(int),SEEK_SET);
                       char dc;
                       char dstr[256];
                       int i=0;

                           while((dc=fgetc(de2_fp))!='\0'){
                           printf("%c\n",dc);
                           dstr[i++] = dc;
                       }
                       printf("%s",str);

                           fclose(de2_fp);

                           goto two;

                       case 3:
                       printf("이전페이지로 이동합니다.\n\n");
                           system("clear");
                           goto one;

                   default:
                       printf("잘못된 입력입니다.\n");
                           system("clear");
                           goto two;
               }
               }

               break;

           case 3:
               printf("\n1. 삽입");
               printf("  2. 복구");
               printf("  3. 이전페이지 : ");
               scanf("%d", &pick3);


               while (1) {
                   switch (pick3) {
                       case 1: //삽입
                            // 데이터 삽입 : lsb변조 : 데이터 길이 삽입 과정과 같음

                                          // file open
                                          en3_fp = fopen("test2.bmp", "r+b");

                                          if (en3_fp == NULL){
                                              printf("file does not exist.\n");
                                              exit(1);
                                          }

                                          else{
                                              printf("input data : "); // 삽입할 data
                                              scanf("%s", input);

                                              input_bit = 8 * strlen(input); // 입력값 길이 * 8 이진수 변환 : 8bits당 1bit 숨기니까
                                              int inputbit_temp = input_bit; // input_bit가 아래서 shift연산 될 때 손상되어서 temp에 저장해둠
                                              // 입력값 길이 2진수로 변환하는 부분 (길이도 삽입할 거라서 제대로 들어가는지 보여주려고)
                                              printf("\n============= data length ==============\n\ninput bit : %d bits= ", input_bit);

                                              for(int i = 7; i >= 0; i--){ // 최상위 비트부터 배열에 순서대로 대입 : >> 7부터 해야 최상위부터 ..
                                                      inputlengtharr[7-i] = (input_bit >> i) & 0x01;// 해당 자리의 비트와 1과 & -> 1이면 1, 0이면 0
                                                      printf("%d",inputlengtharr[7-i]);
                                              }
                                            // input_bit가 아래서 shift연산 될 때 손상되어서 temp에 저장해둠
                                           // input_bit = inputbit_temp;

                                              fseek(en3_fp, 10, SEEK_SET); // header size 읽기 위해 10bytes 이동
                                              fread(&header, 1, 1, en3_fp); // 해당 위치에 저장된 header size 읽음
                                              h = ((header >> 4) * 16) + (header & 0x0f); // header size를 10진수로 바꿔줌
                                              fseek(en3_fp, h, SEEK_SET); // header 뒤쪽으로 이동

                                              printf("\n\n===== hide data length info to LSB =====\n\n");

                                              for(int i = 0 ; i < 8 ; ++i){
                                                  fread(&buffer1, 1, 1, en3_fp); // buffer1에 1byte를 1번 읽어옴 : 커서가 1byte 이동
                                                  printf("original data : %02x", buffer1);

                                                  if((buffer1 & 0x01) != inputlengtharr[i]){ // 숨겨야할 데이터와 파일에서 읽어온 LSB가 다르면 ^0x01(원하는 정보 삽입), 같으면 그대로 사용
                                                      temp = buffer1 ^ 0x01; // 연산한 값을 temp에 저장
                                                      fseek(en3_fp, -1, SEEK_CUR); // 읽었던 자리에 LSB 수정하기 위해 움직인 커서를 다시 1byte 앞으로 옮김
                                                      fwrite(&temp, sizeof(u8), 1, en3_fp); // temp에 저장된 값을 1byte로 씀
                                                  }
                                                  // check re-write : 이부분은 사실 필요 없음
                                                  fseek(en3_fp, -1, SEEK_CUR); // 제대로 쓰여졌는지 다시 출력해보기 위해 1byte 앞으로 이동
                                                  fread(&buffer2, 1, 1, en3_fp); // 정보가 써진 부분 읽어옴
                                                  printf("\nmodified data : %02x", buffer2);
                                                  printf("\n========================================\n");
                                              } // 헤더 입력 완료

                                              // 입력 데이터를 각 문자별로 이진수형태로 (1과0으로 표현) 배열에 저장
                                              for(int i = 0; i < strlen(input) ; ++i){
                                                  for(int j = 0; j < 8 ; ++j){
                                                      data[(8*(i+1)-1) -j] = (input[i] >> j) & 0x01;
                                              }
                                              }
                                            /* check data
                                            printf("print data\n");
                                            for(int i = 0; i<input_bit; ++i){
                                              printf("%d, ",data[i]);
                                            }
                                            */


                              printf("\n\n========= hide data to LSB ==========\n\n");

                              for(int i = 0 ; i < input_bit ; ++i ){ // 총 변조에 필요한 비트 수

                                  fread(&buffer1, 1, 1, en3_fp); // buffer1에 1byte를 1번 읽어옴 : 커서가 1byte 이동

                                  printf("original data : %02x", buffer1);

                                  if((buffer1 & 0x01) != data[i]){ // 숨겨야할 데이터와 파일에서 읽어온 LSB가 다르면 ^0x01(원하는 정보 삽입), 같으면 그대로 사용
                                      temp = buffer1 ^ 0x01; // 연산한 값을 temp에 저장
                                      fseek(en3_fp, -1, SEEK_CUR); // 읽었던 자리에 LSB 수정하기 위해 움직인 커서를 다시 1byte 앞으로 옮김
                                      fwrite(&temp, sizeof(u8), 1, en3_fp); // temp에 저장된 값을 1byte로 씀  // 처음에는 fprintf로 했었는데 그건 파일에 쓰는거라서 쓰는위치와 읽어오는 위치가 달라서 이상햇엇음

                                  }
                                      
                                      fseek(en3_fp, -1, SEEK_CUR); // 제대로 쓰여졌는지 다시 출력해보기 위해 1byte 앞으로 이동
                                      fread(&buffer2, 1, 1, en3_fp); // 정보가 써진 부분 읽어옴

                                      dec_data[i] = buffer2;
                                      printf("\nmodified data: %02x", dec_data[i]);
                                      printf("\n=======================================\n");
                                  }
                                  // 숨기려고 하는 데이터 입력 완료
                           goto two;

                       case 2: //복구

                        // 복호화 : 입력된 데이터 길이는 모르고 헤더 뒤에 길이가 저장되어있는 건 안다고 가정..
                              printf("\n\n======= recovery data from LSB ========\n");
                              fseek(en3_fp, 10, SEEK_SET); // header size 읽기 위해 10bytes 이동
                              fread(&header, 1, 1, en3_fp); // 해당 위치에 저장된 h
                              fseek(en3_fp, h, SEEK_SET); // header 뒤쪽으로 이동

                              for(int i = 0; i < 8; ++i){
                                  fread(&buffer1, 1, 1, en3_fp);
                                  dec_inputlength ^= ((buffer1 & 0x01) << (7-i));
                              }
                              printf("\n\nextracted data length : %d bits", dec_inputlength);  // 데이터 길이 추출 완료 :4글자 입력시 32bits

                              // 숨겨진 데이터 읽는 부분
                              for(int i = 0; i < dec_inputlength/8  ; ++i){ // 입력한 문자의 개수 번 반복
                                  temp2=0; // 문자마다 0으로 다시 초기화
                                  for(int j = 0; j < 8 ; ++j){ // 8bit씩 이라서 8번 반복
                                      fread(&buffer2, 1, 1, en3_fp);
                                      temp2 ^= ((buffer2 & 0x01) << (7-j)); // 0x01과 &연산해서 버퍼의 lsb를 알아낸 후!!
                                                                          // 맨 앞부터 채움 -> 0과 XOR을 8번 반복해서 더해줌
                                                                          // buffer가 01100001 이면 lsb인 1이 숨긴 정보니까 그것을 temp의 최상위 비트로 shift, 그 다음도 동일하게 진행 -> 8번 반복
                                  }
                                  dec_origindata[i] = temp2; // 각 문자의 lsb들을 모은 temp2를 하나의 배열에 넣음
                              }
                                  // 데이터 추출 완료
                              printf("\n\nrecovered data : ");

                              for(int i = 0; i < dec_inputlength/8 ; ++i) // 글자수만큼 출력
                                  printf("%c", dec_origindata[i]);

                          printf("\n=======================================\n\n");
                          fclose(en3_fp);

                           goto two;

                       case 3:
                       printf("이전페이지로 이동합니다.\n\n");
                           system("clear");
                           goto one;

                   default:
                       printf("잘못된 입력입니다.\n");
                           system("clear");
                           goto two;
               }
       }


               break;

           case 4:
               four:
                   printf("\n1. 비교");
                   printf("  2. 이전페이지 : ");

                   scanf("%d", &pick4);

                   while (1) {
                       switch (pick4) {
                           case 1: // diff
                               //사용자가 입력한 이름의 텍스트 파일을
                               //rt모드로 스트림 생성
                               diff_fp1 = fopen("3.jpg", "r+t");
                               diff_fp2 = fopen("4.jpg", "r+t");

                               /* fopen함수는 오류발생시 NULL을 리턴하므로
                               파일 개방 중 오류발생시 프로그램을 종료 */
                               if (diff_fp1 == NULL || diff_fp2 == NULL){
                                    printf("스트림 생성시 오류발생\n");
                                   goto four;
                               }

                               // 두개의 파일에 저장된 데이터를 비교함
                               while (1){
                                    //두개의 파일 모두 끝에 도달하지 않을 경우
                                    if (feof(diff_fp1) == 0 && feof(diff_fp2) == 0){
                                         diff_a = fgetc(diff_fp1);
                                         diff_b = fgetc(diff_fp2);


                                         if (diff_a != diff_b){
                                           printf("다른 부분입니다.\n");
                                           printf("A :%c /",diff_a);
                                           printf(" B :%c\n\n",diff_b);
                                           diff_count++;
                                         }
                                    }
                                    //하나의 파일만 끝에 도달할 경우
                                    else if (feof(diff_fp1) != 0 && feof(diff_fp2) == 0){

                                      while (feof(diff_fp2)==0) {
                                        diff_a = fgetc(diff_fp2);
                                        printf("A :%c", diff_a);
                                      }

                                         printf("두개의 파일은 일치하지 않습니다.\n");
                                         goto four;
                                    }

                                    //하나의 파일만 끝에 도달할 경우
                                    else if (feof(diff_fp1) == 0 && feof(diff_fp2) != 0){

                                        while (feof(diff_fp1)==0) {
                                          diff_b = fgetc(diff_fp1);
                                          printf("B :%c\n", diff_b);
                                        }

                                         printf("두개의 파일은 일치하지 않습니다.\n");
                                        goto four;
                                    }

                                    //두개의 파일 모두 끝에 도달한 경우.
                                    //(첫 번째 조건문에서 각 파일의 문자는 검사했기 때문에
                                    //두 파일이 동시에 feof에 의해 탈출하면 동일한 파일인 것)
                                    else{

                                      if(diff_count == 0){
                                         printf("두개의 파일은 일치합니다.\n");
                                        goto four;
                                       }
                                       printf("\n\n");
                                       printf("================================\n");
                                       printf("\n두개의 파일은 일치하지 않습니다.\n\n");
                                       goto four;
                                    }
                               }

                               /* fclose함수는 종료시 오류가 발생하면
                               0이 아닌 다른값을 리턴하므로 비정상 종료로 판단되면
                               안내후 프로그램을 종료 */
                               diff_state1 = fclose(diff_fp1);
                               diff_state2 = fclose(diff_fp2);

                               if (diff_state1 != 0 || diff_state2 != 0){
                                    printf("스트림 제거시 오류발생\n");
                                   goto four;
                               }

                           case 2:
                           printf("이전페이지로 이동합니다.\n\n");
                               system("clear");
                               goto one;

                       default:
                           printf("잘못된 입력입니다.\n");
                               system("clear");
                               goto two;
                   }
                   }



               break;

           case 5:
               printf("종료합니다.\n");
               exit(0);

           default:
               printf("잘못된 입력입니다.\n\n\n");
               goto one;

       }
}
     }
}
