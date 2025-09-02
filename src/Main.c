#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/GSprite.h"
#include "/home/codeleaded/System/Static/Library/NeuralNetwork.h"
#include "/home/codeleaded/System/Static/Library/AffineTransform.h"

#define SPRITE_PATH         "/home/codeleaded/Data/NN/Digits/"
#define DATA_PATH           "/home/codeleaded/Data/NN/DigitsGray/"
#define SPRITE_TEST         "testing"
#define SPRITE_TRAINING     "training"
#define SPRITE_COUNT        1
#define SPRITE_MAX          5000

#define NN_PATH             "./data/Model.nnalx"
#define NN_COUNT            10
#define NN_LEARNRATE        0.5f

unsigned int ndir = 0U;
unsigned int item = 0U;

float x;
float y;
float s;
float a;

AffineTransform transform;
GSprite sp;

void Setup(AlxWindow* w){
    RGA_Set(Time_Nano());

    ResizeAlxFont(8,8);

    x = 0.0f;
    y = 0.0f;
    s = 1.0f;
    a = 0.0f;

    transform = AffineTransform_New();
    sp = GSprite_None();
}
void Update(AlxWindow* w){
    if(Stroke(ALX_KEY_R).PRESSED){
        ndir = Random_u32_MinMax(0,10);
        item = Random_u32_MinMax(0,SPRITE_MAX);

        CStr ntraining_s = CStr_Format("%s/%d/%d.sprg",DATA_PATH SPRITE_TEST,ndir,item);
        GSprite_Free(&sp);
        sp = GSprite_Load(ntraining_s);
        CStr_Free(&ntraining_s);
    }

    if(Stroke(ALX_KEY_W).DOWN)      y -= 10.0f * w->ElapsedTime;
    if(Stroke(ALX_KEY_S).DOWN)      y += 10.0f * w->ElapsedTime;
    if(Stroke(ALX_KEY_A).DOWN)      x -= 10.0f * w->ElapsedTime;
    if(Stroke(ALX_KEY_D).DOWN)      x += 10.0f * w->ElapsedTime;
    
    if(Stroke(ALX_KEY_UP).DOWN)     s -= 1.0f * w->ElapsedTime;
    if(Stroke(ALX_KEY_DOWN).DOWN)   s += 1.0f * w->ElapsedTime;
    
    if(Stroke(ALX_KEY_LEFT).DOWN)   a -= 1.0f * w->ElapsedTime;
    if(Stroke(ALX_KEY_RIGHT).DOWN)  a += 1.0f * w->ElapsedTime;

    Clear(DARK_BLUE);

    const float px = 10.0f;
    const float py = 10.0f;

    AffineTransform_Identity(&transform);
    AffineTransform_Trans(&transform,(Vec2){ x,y });
    AffineTransform_Trans(&transform,(Vec2){ -(float)sp.w * 0.5f,-(float)sp.h * 0.5f });
    //AffineTransform_Trans(&transform,(Vec2){ -x,-y });
    AffineTransform_RotateZ(&transform,a);
    AffineTransform_Trans(&transform,(Vec2){ (float)sp.w * 0.5f,(float)sp.h * 0.5f });
    AffineTransform_Scale(&transform,(Vec2){ s,s });
    Rect area = AffineTransform_Rect(&transform,(Rect){ 0.0f,0.0f,sp.w,sp.h });
    AffineTransform_Inverse(&transform);

    for(int i = 0;i<area.d.y;i++){
        for(int j = 0;j<area.d.x;j++){
            const Vec2 rp = { j,i };
            const Vec2 p = AffineTransform_Calc(&transform,rp);
            const float l = GSprite_Get(&sp,p.x,p.y);
            
            if(l>=0.0f && l<=1.0f){
                const Pixel col = Pixel_toRGBA(l,l,l,1.0f);
                RenderPixel(px + rp.x,py + rp.y,col);
            }
        }
    }

    //String str = String_Format("T:%d, ND:%d, I:%d",test,ndir,item);
    //RenderCStrSize(str.Memory,str.size,0.0f,0.0f,WHITE);
    //String_Free(&str);

    String str = String_Format("N: %d, I: %d",ndir,item);
    RenderCStrSize(str.Memory,str.size,0.0f,0.0f,WHITE);
    String_Free(&str);

    str = String_Format("X: %f, Y: %f, S: %f, A: %f",x,y,s,a);
    RenderCStrSize(str.Memory,str.size,0.0f,GetAlxFont()->CharSizeY,WHITE);
    String_Free(&str);
}
void Delete(AlxWindow* w){
    GSprite_Free(&sp);
}

int main(){
    if(Create("RGB to G",160,120,8,8,Setup,Update,Delete))
        Start();
    return 0;
}