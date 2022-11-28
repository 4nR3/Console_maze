#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <math.h>

Adafruit_ILI9341 tft = Adafruit_ILI9341(8, 9, 11, 13, 10, 12);

int y = 15, op;
int dv = y + 2;
int tamx, tamy,dotx,doty,salix,saliy,startx,starty;
int matriz[17][17];
int dibux = tft.width() / dv, dibuy = tft.height() / dv;
bool termine=false;

void arbol(int lx, int ly);
void pasto1(int lx, int ly);
void pasto2(int lx, int ly);
void flor1(int lx, int ly);
void flor2(int lx, int ly);
void personaje (int lx, int ly);
void terminar ();
void destruirparedes(int x,int y);
void generarsalida();

double mini=0;
int xbreak,ybreak;

long randNumber;

struct assets {

  String linea1;
  String linea2;
  String linea3;
};

assets celdas[16];
int nivel=1;
String adyacencia[17];

void setup() {
  
  int f,i;

  randomSeed(analogRead(0));

  tft.begin();

  tamx = dibux / 5;
  tamy = dibuy / 5;

  tft.fillScreen(ILI9341_GREEN);


  celdas[0].linea1= "o o";
  celdas[0].linea2= "   ";
  celdas[0].linea3= "o o";
  
  celdas[1].linea1= "o o";
  celdas[1].linea2= "o o";
  celdas[1].linea3= "o o";
  
  celdas[2].linea1= "ooo";
  celdas[2].linea2= "   ";
  celdas[2].linea3= "ooo";
  
  celdas[3].linea1= "ooo";
  celdas[3].linea2= "o  ";
  celdas[3].linea3= "o o";
  
  celdas[4].linea1= "o o";
  celdas[4].linea2= "o  ";
  celdas[4].linea3= "ooo";
  
  celdas[5].linea1= "o o";
  celdas[5].linea2= "  o";
  celdas[5].linea3= "ooo";
  
  celdas[6].linea1= "ooo";
  celdas[6].linea2= "  o";
  celdas[6].linea3= "o o";
  
  celdas[7].linea1= "o o";
  celdas[7].linea2= "   ";
  celdas[7].linea3= "ooo";
  
  celdas[8].linea1= "o o";
  celdas[8].linea2= "  o";
  celdas[8].linea3= "o o";
  
  celdas[9].linea1= "ooo";
  celdas[9].linea2= "   ";
  celdas[9].linea3= "o o";
  
  celdas[10].linea1= "o o";
  celdas[10].linea2= "o  ";
  celdas[10].linea3= "o o";
  
  celdas[11].linea1= " o ";
  celdas[11].linea2= " o ";
  celdas[11].linea3= " o ";
  
  celdas[12].linea1= "ooo";
  celdas[12].linea2= " o ";
  celdas[12].linea3= "   ";
  
  celdas[13].linea1= "   ";
  celdas[13].linea2= " o ";
  celdas[13].linea3= "ooo";
  
  celdas[14].linea1= "  o";
  celdas[14].linea2= " oo";
  celdas[14].linea3= "  o";
  
  celdas[15].linea1= "o  ";
  celdas[15].linea2= "oo ";
  celdas[15].linea3= "o  ";

  adyacencia[0]= "ooooooooooooooooo";
  adyacencia[16]= "ooooooooooooooooo";
  
  for(nivel=1;nivel<=15;nivel+=3)
    {
    adyacencia[nivel]+="o";
    adyacencia[nivel+1]+="o";
    adyacencia[nivel+2]+="o";
    
    for(int f=0;f<5;f++)
        {
         randNumber = random(16);
               
         adyacencia[nivel]+=celdas[randNumber].linea1;
         adyacencia[nivel+1]+=celdas[randNumber].linea2;
         adyacencia[nivel+2]+=celdas[randNumber].linea3;
         }
      
      adyacencia[nivel]+="o";
      adyacencia[nivel+1]+="o";
      adyacencia[nivel+2]+="o";
    }

    generarsalida();
    
    int cont=0;
    
  for(f=0;f<y+2;f++)
    {for(i=0;i<y+2;i++)
      {if(adyacencia[f][i]=='o')
        matriz[f][i]=2;
       else
      if(adyacencia[f][i]==' ')
        matriz[f][i]=0;
       else
     if(adyacencia[f][i]=='E')
        {matriz[f][i]=3;
        startx=dotx=f;
        starty=doty=i;}
      else
     if(adyacencia[f][i]=='S')
        {matriz[f][i]=4;
        salix=f;
        saliy=i;}
   
      }
    }
  
  while(termine==false){
  
  mini=0;

  dfs(dotx,doty);
  int distanciay,distanciax,movimiento;

  distanciax=max(xbreak,salix)-min(xbreak,salix);
  distanciay=max(ybreak,saliy)-min(ybreak,saliy);

  if(distanciay <= distanciax)
        {if(salix-xbreak <= 0)
            movimiento=4;
        else
            movimiento=3;
        }
    else
        {if(saliy-ybreak <= 0)
            movimiento=2;
        else
            movimiento=1;
        }

  dotx=xbreak;
  doty=ybreak;
  
  destruirparedes(xbreak,ybreak,movimiento);

    }

  for (f = 0; f < y + 2; f++)
  {
    for (i = 0; i < y + 2; i++)
    { if (adyacencia[f][i] == 'o')
      {
        if (cont % 5 == 0)
          arbol(i * dibux, f * dibuy);
        else 
        if (cont % 5 == 1)
          pasto1(i * dibux, f * dibuy);
        else 
        if (cont % 5 == 2)
          flor2(i * dibux, f * dibuy);
        else 
        if (cont % 5 == 3)
          flor1(i * dibux, f * dibuy);
        else 
        if (cont % 5 == 4)
          pasto2(i * dibux, f * dibuy);

      cont++;
      }
      else if (adyacencia[f][i] == ' ' || adyacencia[f][i] == 'E')
        {tft.fillRect((i * dibux)-tamx, (f * dibuy)-tamy, tamx * 8, tamy * 6 + 1, tft.color565(98, 98, 71));

        }
      else if (adyacencia[f][i] == 'S')
        {meta(i * dibux, f * dibuy);}

    }
    
  }

  cont=0;


  dotx=startx;
  doty=starty;
   
  personaje((doty * dibux),(dotx * dibuy));
    
    
}

bool findfs=false;

void dfs (int x,int y){

  if(findfs==true)
    return;
  
  int cont=0;

  int posx[]={1,0,-1,0};
  int posy[]={0,1,0,-1};
  int auxy=17;

  for(int f=0;f<4;f++)
      {if(x+posx[f]<auxy && x+posx[f]>=0 && y+posy[f]<auxy && y+posy[f]>=0 )
          {if(matriz[x+posx[f]][y+posy[f]]==0)
              {matriz[x+posx[f]][y+posy[f]]=1;
               cont++;
               dfs(x+posx[f],y+posy[f]);}
          }

       if(x+posx[f]==salix && y+posy[f]==saliy)
            {findfs=true;
            termine=true;
            return;}
           
      }
      
      if(findfs==true)
         return;
      
      if(cont==0)
        {
        int x2=salix,y2=saliy,f;
        double res;

        res=sqrt((pow((x2-x),2)+pow((y2-y),2)));
  
        if(mini==0 || res<mini)
          {mini=res;
          xbreak=x;
          ybreak=y;}
        
        }  
}


void destruirparedes(int xbreak, int ybreak, int movimiento){

  for(int f=0;f<4;f++)
        {delay(250);
         switch (movimiento)
            {
                case 1:{
                        if(ybreak+f<y+1)
                            {adyacencia[xbreak][ybreak+f]=' ';
                            matriz[xbreak][ybreak+f]=0;
                            }
                        break;}

                case 2:{if(ybreak-f>0)
                            {adyacencia[xbreak][ybreak-f]=' ';
                            matriz[xbreak][ybreak-f]=0;
                            }
                        break;}

                case 3:{if(xbreak+f<y+1)
                           {adyacencia[xbreak+f][ybreak]=' ';
                            matriz[xbreak+f][ybreak]=0;
                            }
                        break;}

                case 4:{if(xbreak-f>0)
                            {adyacencia[xbreak-f][ybreak]=' ';
                            matriz[xbreak-f][ybreak]=0;
                            }
                        break;}
            }
        }
}

void loop() {
  /*sensor.getAcceleration(&accelx, &accely, &accelz);
   sensor.getRotation(&gyrox, &gyroy, &gyroz);
  */
  
   int op,movimientoace,movimientogyro;

   /*movimientoace=accely;
   movimientogyro=gyrox;

   ///movimientoace=accelx;
   ///movimientoace=accelz;
   
   ///movimientogyro=gyroy;
   ///movimientogyro=gyroz;

    if(movimientogyro<=-30)
      op=97;
    else
    if(movimientogyro>=30)
      op=100;
    else
    if(movimientoace<=-30)
      op=119;
    else
    if(movimientoace>=30)
      op=115;
   

     if(digitalRead(2)==HIGH)
       {op=27;}
    */

    if(digitalRead(6)==HIGH) 
       {      if(doty-1>=0)
                if(adyacencia[dotx][doty-1]==' '||adyacencia[dotx][doty-1]=='E'||adyacencia[dotx][doty-1]=='S')
                  {tft.fillRect((doty * dibux)-tamx, (dotx * dibuy)-tamy, tamx * 8, tamy * 6 + 1, tft.color565(98, 98, 71));
                     doty--;
                     personaje((doty * dibux),(dotx * dibuy));     
                  }
                  
       delay(500);}
       
    if(digitalRead(7)==HIGH) 
       {if(dotx-1>=0)
            if(adyacencia[dotx-1][doty]==' '||adyacencia[dotx-1][doty]=='E'||adyacencia[dotx-1][doty]=='S')
                            { tft.fillRect((doty * dibux)-tamx, (dotx * dibuy)-tamy, tamx * 8, tamy * 6 + 1, tft.color565(98, 98, 71));
                                 dotx--;
                                 personaje((doty * dibux),(dotx * dibuy));
                            }
       delay(500);}
       
    if(digitalRead(4)==HIGH) 
       {if(doty+1<y+2)
          if(adyacencia[dotx][doty+1]==' '||adyacencia[dotx][doty+1]=='E'||adyacencia[dotx][doty+1]=='S')
                            { tft.fillRect((doty * dibux)-tamx, (dotx * dibuy)-tamy, tamx * 8, tamy * 6 + 1, tft.color565(98, 98, 71));
                                 doty++;
                                 personaje((doty * dibux),(dotx * dibuy));
                            }
       delay(500);}
       
    if(digitalRead(5)==HIGH) 
       {if(dotx+1<y+2)
               if(adyacencia[dotx+1][doty]==' '||adyacencia[dotx+1][doty]=='E'||adyacencia[dotx+1][doty]=='S')
                        {    tft.fillRect((doty * dibux)-tamx, (dotx * dibuy)-tamy, tamx * 8, tamy * 6 + 1, tft.color565(98, 98, 71));
                             dotx++;
                             personaje((doty * dibux),(dotx * dibuy));
                             }
       delay(500);}

            if(dotx==salix && doty==saliy)
              {terminar();}
    
}

void generarsalida(){

    int pared1,pared2,entrada,salida,f;

    entrada=random(1,16);
    salida=random(1,16);

    pared1=random(1,5);

    for(f=0;f<4;f++)
        {switch (pared1)
            {
                case 1:{adyacencia[entrada][f]=' ';
                        adyacencia[entrada][0]='E';
                        pared2=2;
                        break;}

                case 2:{adyacencia[entrada][y+1]='E';
                        adyacencia[entrada][y+1-f]=' ';
                        pared2=1;
                        break;}

                case 3:{adyacencia[f][entrada]=' ';
                        adyacencia[0][entrada]='E';
                        pared2=4;
                        break;}

                case 4:{adyacencia[y+1-f][entrada]=' ';
                        adyacencia[y+1][entrada]='E';
                        pared2=3;
                        break;}

                }

        }

    for(f=0;f<4;f++)
        {switch (pared2)
            {
                case 1:{adyacencia[salida][f]=' ';
                        adyacencia[salida][0]='S';
                        break;}

                case 2:{adyacencia[salida][y+1-f]=' ';
                        adyacencia[salida][y+1]='S';
                        break;}

                case 3:{adyacencia[f][salida]=' ';
                        adyacencia[0][salida]='S';
                        break;}

                case 4:{adyacencia[y+1-f][salida]=' ';
                        adyacencia[y+1][salida]='S';
                        break;}
                }
        }
  }


void arbol(int lx, int ly) {

  tft.fillRect(lx + (tamx * 1), ly + 0, tamx * 4, tamy * 1, tft.color565(40, 100, 0));
  tft.fillRect(lx + (tamx * 0), ly + (tamy * 1), tamx * 5, tamy * 2, tft.color565(40, 100, 0));
  tft.fillRect(lx + (tamx * 2), ly + (tamy * 3), tamx * 2, tamy * 2, tft.color565(53, 33, 4));

}

void pasto1(int lx, int ly) {

  tft.fillRect(lx + 0, ly + (tamy * 1), tamx * 1, tamy * 4, tft.color565(20, 100, 0));
  tft.fillRect(lx + (tamx * 1), ly + (tamy * 2), tamx * 1, tamy * 3, tft.color565(20, 100, 0));
  tft.fillRect(lx + (tamx * 2), ly + (tamy * 4), tamx * 1, tamy * 1, tft.color565(20, 100, 0));
  tft.fillRect(lx + (tamx * 3), ly + (tamy * 3), tamx * 2, tamy * 2, tft.color565(20, 100, 0));
  tft.fillRect(lx + (tamx * 5), ly + (tamy * 2), tamx * 1, tamy * 3, tft.color565(20, 100, 0));

}

void pasto2(int lx, int ly) {

  tft.fillRect(lx + 0, ly + (tamy * 2), tamx * 5, tamy * 3, tft.color565(20, 100, 0));
  tft.fillRect(lx + (tamx * 1), ly + (tamy * 2), tamx * 3, tamy * 1, ILI9341_GREEN);
  tft.fillRect(lx + (tamx * 2), ly + (tamy * 3), tamx * 1, tamy * 1, ILI9341_GREEN);
}

void flor1(int lx, int ly) {

  tft.fillRect(lx + (tamx * 2), ly + 0, tamx * 1, tamy * 3, ILI9341_RED);
  tft.fillRect(lx + (tamx * 1), ly + (tamy * 1), tamx * 3, tamy * 1, ILI9341_RED);
  tft.fillRect(lx + (tamx * 2), ly + (tamy * 1), tamx * 1, tamy * 1, ILI9341_YELLOW);
  tft.fillRect(lx + (tamx * 2), ly + (tamy * 3), tamx * 1, tamy * 2, tft.color565(0,100,50));

}


void flor2(int lx, int ly) {

  int col1 = random(101);
  int col2 = random(101);
  int col3 = random(101);


  tft.fillRect(lx + (tamx * 1), ly + 0, tamx * 1, tamy * 3, ILI9341_MAGENTA);
  tft.fillRect(lx + 0, ly + (tamy * 1), tamx * 3, tamy * 1, ILI9341_MAGENTA);
  tft.fillRect(lx + (tamx * 3), ly + (tamy * 2), tamx * 1, tamy * 3, ILI9341_MAGENTA);
  tft.fillRect(lx + (tamx * 2), ly + (tamy * 3), tamx * 3, tamy * 1, ILI9341_MAGENTA);

  tft.fillRect(lx + (tamx * 1), ly + (tamy * 1), tamx * 1, tamy * 1, ILI9341_YELLOW);
  tft.fillRect(lx + (tamx * 3), ly + (tamy * 3), tamx * 1, tamy * 1, ILI9341_YELLOW);

}

void meta (int lx, int ly) {

  tft.fillRect(lx + 0, ly + 0, tamx * 5, tamy * 5, tft.color565(100, 75, 0));

  tft.fillRect(lx + 0, ly + 0, tamx * 5, tamy * 3, ILI9341_BLACK);

  tft.fillRect(lx + (tamx * 1), ly + 0, tamx * 1, tamy * 1, ILI9341_WHITE);
  tft.fillRect(lx + (tamx * 3), ly + 0, tamx * 1, tamy * 1, ILI9341_WHITE);
  tft.fillRect(lx + 0, ly + (tamy * 1), tamx * 1, tamy * 1, ILI9341_WHITE);
  tft.fillRect(lx + (tamx * 2), ly + (tamy * 1), tamx * 1, tamy * 1, ILI9341_WHITE);
  tft.fillRect(lx + (tamx * 4), ly + (tamy * 1), tamx * 1, tamy * 1, ILI9341_WHITE);
  tft.fillRect(lx + (tamx * 1), ly + (tamy * 2), tamx * 1, tamy * 1, ILI9341_WHITE);
  tft.fillRect(lx + (tamx * 3), ly + (tamy * 2), tamx * 1, tamy * 1, ILI9341_WHITE);

  tft.fillRect(lx + 0, ly + (tamy * 3), tamx * 1, tamy * 2, tft.color565(53, 33, 4));
  tft.fillRect(lx + (tamx * 4), ly + (tamy * 3), tamx * 1, tamy * 2, tft.color565(53, 33, 4));
}

void personaje(int lx,int ly){

    tft.fillRect(lx+(tamx * 1),ly,tamx * 4,tamy * 5,ILI9341_BLUE);
    tft.fillRect(lx,ly+(tamy * 1),tamx * 6,tamy * 3,ILI9341_BLUE);

    tft.fillRect(lx+(tamx * 1),ly+(tamy*1),tamx * 1,tamy * 1,ILI9341_WHITE);
    tft.fillRect(lx+(tamx * 4),ly+(tamy*1),tamx * 1,tamy * 1,ILI9341_WHITE);
    tft.fillRect(lx+(tamx * 2),ly+(tamy*3),tamx * 2,tamy * 1,ILI9341_WHITE);

}

void terminar (){

  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(tft.width() / 2-40,tft.height()/2);
  tft.setTextColor(ILI9341_WHITE); 
  tft.setTextSize(1);
  tft.println("GRACIAS POR JUGAR");

  delay(5000);

}
