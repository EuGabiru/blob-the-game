#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/keyboard.h>
#include <iostream>
#include <math.h>
#include "projeteis.h"

// 0 -> Parado esquerda
// 1 -> Correndo esquerda
// 2 -> Correndo + Atirando de lado (esquerda)
// 3 -> Atirando de lado (esquerda)
// 4 -> Parado de frente
// 5 -> Correndo de frente
// 6 -> Correndo + Atirando de frente
// 7 -> Atirando de Frente
// 8 -> Parado de costas
// 9 -> Correndo de costas


//tamanho X:16 >largura
//mamanho Y: 32 > altura

void spawn(int &pos_inimigo_x, int &pos_inimigo_y, int x, int y) {
    while(x+128 == pos_inimigo_x && y+128 == pos_inimigo_y) {
        pos_inimigo_x = rand() % 300;
        pos_inimigo_y = rand() % 300;
    }
}

//enum do vetor de teclas
enum {CIMA,BAIXO,DIREITA,ESQUERDA,setaBAIXO,setaCIMA,setaESQUERDA,setaDIREITA};
enum{inimigoESQ,inimigoDIR,inimigoBAIXO,inimigoCIMA};

int main()
{
   //INICIALIZAÇÃO DAS BIBLIOTECAS
    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_install_keyboard();
    al_init_primitives_addon();





   //DISPLAY EM TELA CHEIA
   ALLEGRO_DISPLAY *display = NULL;
   //  al_set_window_position(display, 300, 300);

  int autura_t = 1024/2 ;
  int largura_t = 720/2
   ;

   ALLEGRO_MONITOR_INFO info;


   int res_x_comp,res_y_comp;
   al_init_font_addon();

   int contador = 0;

   al_get_monitor_info(0, &info);
   res_x_comp = info.x2 - info.x1;
   res_y_comp = info.x2 - info.x1;

   al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
   display = al_create_display(res_x_comp, res_y_comp);

   float red_x = res_x_comp / (float) largura_t;
   float red_y = res_y_comp / (float) autura_t;

   ALLEGRO_TRANSFORM transf;



   al_identity_transform(&transf);
   al_scale_transform(&transf, red_x, red_y);
   al_use_transform(&transf);


   al_set_window_title(display,"JOGO MAIS FODA Q TEM");


   //SPRITES NO GERAL
    ALLEGRO_FONT* font = al_load_font("./Monocraft.ttf",20,0);
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 20.0);

    ALLEGRO_BITMAP* spritePlayer = al_load_bitmap ("./blob-sprites.png");
    ALLEGRO_BITMAP* spriteTiro = al_load_bitmap ("./tiro-player.png");
    ALLEGRO_BITMAP* spriteInimigo = al_load_bitmap("./inimigo.png");
   //ALLEGRO_BITMAP *bg = al_load_bitmap("./Backgroud.png");

      //FILA DE EVENTOS
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_start_timer(timer);


   //CONTROLE DA DIREÇÃO/CONTROLE DOS SPRITES
    float frame = 0.f;
    int pos_x = 165, pos_y = 110;
    int pos_tiro_x = 0, pos_tiro_y = 0;
    int current_frame_y = 32;
    int current_frame_disparos_y = 32;
    bool teclas[] = {false,false,false,false,false,false,false,false};
    bool direita = false,setadireita = false,setacima = false;
    int pos_inimigo_x = rand() % largura_t, pos_inimigo_y = rand() % autura_t, current_frame_inimigo_y = 0;
    bool inimigo[] = {true,true};
    bool rodando = true, direitaInimigo = true;

   //MOVIMENTAÇÃO DO PLAYER NO GERAL
    while (rodando)
    {    ALLEGRO_EVENT event;

          al_wait_for_event(event_queue, &event);
         if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
         {
            break;
         }
         //MOVIMENTAÇÃO DO PLAYER
         if(event.type == ALLEGRO_EVENT_KEY_DOWN){
         switch (event.keyboard.keycode){
            case ALLEGRO_KEY_W:
                    current_frame_y = 32 * 9;
                    teclas[CIMA] = true;
                    direita = false;
                break;

            case ALLEGRO_KEY_A:
                    current_frame_y = 32 * 1;
                    teclas[ESQUERDA] = true;
                    direita = false;

                break;

            case ALLEGRO_KEY_S:
                    current_frame_y = 32 * 5;
                    teclas[BAIXO] = true;
                    direita = false;

                break;

            case ALLEGRO_KEY_D:
                    current_frame_y = 32 * 1;
                    teclas[DIREITA] = true;
                    direita = true;

                break;
            case ALLEGRO_KEY_LEFT:
                    current_frame_disparos_y = 32 * 0;
                    current_frame_y = 32 * 3;
                    direita = false;
                    setadireita = false;
                    teclas[setaESQUERDA] = true;
                    teclas[setaBAIXO] = false;
                    teclas[setaCIMA] = false;
                    teclas[setaDIREITA] = false;
                    pos_tiro_y = pos_y;
                    pos_tiro_x = pos_x;
                break;
            case ALLEGRO_KEY_RIGHT:
                    current_frame_y = 32 * 3;
                    current_frame_disparos_y = 32 * 0;
                    direita = true;
                    setadireita = true;
                    teclas [setaDIREITA] = true;
                    pos_tiro_x = pos_x;
                    pos_tiro_y = pos_y;
                    teclas[setaBAIXO] = false;
                    teclas[setaCIMA] = false;
                    teclas[setaESQUERDA] = false;
                break;
            case ALLEGRO_KEY_UP:
                    current_frame_y = 32 * 8;
                     current_frame_disparos_y = 32 * 0;
                     teclas[setaCIMA] = true;
                     setadireita = false;
                     setacima = true;
                     pos_tiro_y = pos_y;
                    pos_tiro_x = pos_x;
                    teclas[setaBAIXO] = false;
                    teclas[setaESQUERDA] = false;
                    teclas[setaDIREITA] = false;
                break;
            case ALLEGRO_KEY_DOWN:
                    current_frame_y = 32 * 7;
                    current_frame_disparos_y = 32 * 0;
                    teclas[setaBAIXO] = true;
                    setadireita = false;
                    pos_tiro_y = pos_y;
                    pos_tiro_x = pos_x;
                    teclas[setaCIMA] = false;
                    teclas[setaDIREITA] = false;
                    teclas[setaESQUERDA] = false;
                break;
         }
         }

            //ANIMAÇÃO DO PLAYER PARDO E ATIRANDO MUITO LEGAL MSM



         //ANIMAÇÃO DO PLAYER ANDANDO E ATIRANDO DEU ME AJUDE POR FAVOR



         if(event.type == ALLEGRO_EVENT_KEY_UP){

            switch (event.keyboard.keycode )
            {

            case ALLEGRO_KEY_W:
               teclas[CIMA] = false;
               current_frame_y = 32 * 8;
               break;
            case ALLEGRO_KEY_S:
            current_frame_y = 32 * 4;
               teclas[BAIXO] = false;
               break;
            case ALLEGRO_KEY_A:
               teclas[ESQUERDA] = false;
               direita = false;
               current_frame_y = 32 * 0;
               break;
            case ALLEGRO_KEY_D:
               teclas[DIREITA] = false;
               direita = true;
               current_frame_y = 32 * 0;
                break;
            case ALLEGRO_KEY_DOWN:
                current_frame_y = 32*4;
                break;
            case ALLEGRO_KEY_UP:
                break;
            case ALLEGRO_KEY_LEFT:
                current_frame_y = 32 * 0;
                break;
            case ALLEGRO_KEY_RIGHT:
                direita = true;
                setadireita = true;
                current_frame_y = 32 * 0;
                break;
            case ALLEGRO_KEY_ESCAPE:
                rodando = false;
                break;
            }
            }
//PROGRAMAÇÃO DO INIMIGO O CARA É BOM DEMAIS
    if (pos_x != pos_inimigo_x) {
        if(pos_x > pos_inimigo_x) {
            direitaInimigo = true;
            pos_inimigo_x += 1;
        } else if(pos_x < pos_inimigo_x) {
            pos_inimigo_x -= 1;
            direitaInimigo = false;
        }

    }

    if (pos_y != pos_inimigo_y) {
        if(pos_y > pos_inimigo_y) {
            pos_inimigo_y += 1;
        } else if(pos_y < pos_inimigo_y) {
            pos_inimigo_y -= 1;
        }

    }


    if (pos_x < pos_inimigo_x + 16 &&
            pos_x + 16 > pos_inimigo_x &&
            pos_y < pos_inimigo_y + 16 &&
            pos_y + 16 > pos_inimigo_y) {

            rodando = false;
    }

    if (pos_tiro_x < pos_inimigo_x + 20 &&
            pos_tiro_x + 10 > pos_inimigo_x &&
            pos_tiro_y < pos_inimigo_y + 20 &&
            pos_tiro_y + 10 > pos_inimigo_y) {
            contador++;
            pos_inimigo_x = rand() % 300;
            pos_inimigo_y = rand() % 300;
            spawn(pos_inimigo_x, pos_inimigo_y, pos_x-32, pos_y-32);

    }





         //CONTROLE DOS VETORES DE MOVIMENTAÇÃO
         pos_y -= teclas[CIMA] *2;
         pos_y += teclas[BAIXO] *2;
         pos_x -= teclas[ESQUERDA] *2;
         pos_x += teclas [DIREITA] *2;
         pos_tiro_y += teclas[setaBAIXO] *10;
         pos_tiro_y -= teclas[setaCIMA] *10;
         pos_tiro_x += teclas[setaDIREITA] *10;
         pos_tiro_x -= teclas[setaESQUERDA] *10;



         if(pos_tiro_y > autura_t){
         teclas[setaBAIXO] = false;
         teclas[setaCIMA] = false;
         }

         if (pos_tiro_x >largura_t){
            teclas[setaESQUERDA] = false;
            teclas[setaDIREITA] = false;

         }


         //"CONTROLADOR DOS FRAMES"
         frame += 0.3f;
         if(frame > 3){
            frame -= 3;
         }


         //"DESENHAR" O BACKGROUND E TODO O RESTO

         al_clear_to_color(al_map_rgb(53, 43, 48));

         //al_draw_bitmap(bg,0,0,0);
          al_draw_bitmap_region(spritePlayer,32 * (int)frame ,current_frame_y , 32 , 32,pos_x ,pos_y ,direita ? ALLEGRO_FLIP_HORIZONTAL:0);
          al_draw_bitmap_region(spriteTiro,32 * (int)frame,current_frame_disparos_y,32 , 32,pos_tiro_x,pos_tiro_y, setadireita ? ALLEGRO_FLIP_HORIZONTAL: 0);
          al_draw_bitmap_region(spriteInimigo,32 * (int)frame,current_frame_inimigo_y,32,32,pos_inimigo_x,pos_inimigo_y,direitaInimigo ? ALLEGRO_FLIP_HORIZONTAL:0);

           //al_draw_text(font, al_map_rgb(0, 0, 0), 12, 12, 0, "Score: sim :)");


        al_draw_textf(font, al_map_rgb(230, 212, 190), 10, 20, 0, "Kills: %i" , contador);
         al_flip_display();


    }
    al_clear_to_color(al_map_rgb(53, 43, 48));
    al_draw_textf(font, al_map_rgb(230,212, 190), 110, 110, 0, "KILLS: %i" , contador);
    al_flip_display();
    al_rest(2);
    //DESTRUINDO TUDO
    //al_destroy_bitmap(bg);
    al_destroy_bitmap(spriteInimigo);
    al_destroy_bitmap(spriteTiro);
    al_destroy_bitmap(spritePlayer);
    //al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
    }
