#include "Task_LVGL_Tick.h"
#include "lvgl.h"
#include "lv_demo_stress.h"

// ���ȫ�ֱ������ڶ���
static lv_obj_t * chart;
static lv_chart_series_t * ser1;
static lv_obj_t * arc;
static lv_obj_t * label;
static lv_obj_t * bar;
static lv_obj_t * slider;
static int chart_index = 0;

// ͼ����¶�ʱ���ص�
static void update_chart_timer(lv_timer_t * timer)
{
    // ����µ����ݵ㵽ͼ��
    static int cnt = 0;
    int next_value = (lv_rand(0, 100) + lv_rand(0, 100)) / 2;
    lv_chart_set_next_value(chart, ser1, next_value);
    
    // ���±�ǩ��ʾ
    static char buf[32];
    lv_snprintf(buf, sizeof(buf), "Value: %d", next_value);
    lv_label_set_text(label, buf);
    
    cnt++;
    if(cnt > 100) cnt = 0;
}

// ���ν����������ص�
static void arc_loader(lv_timer_t * timer)
{
    static int16_t a = 0;
    a += 5;
    if(a >= 360) a = 0;
    
    lv_arc_set_value(arc, a);
    
    // ���»��α�ǩ
    static char buf[32];
    lv_snprintf(buf, sizeof(buf), "%d%%", a * 100 / 360);
    lv_label_set_text(label, buf);
}

// ���鶯���ص�
static void slider_anim(void * var, int32_t value)
{
    lv_slider_set_value(slider, value, LV_ANIM_OFF);
    
    // ���ݻ���ֵ�ı��ǩ��ɫ
    lv_color_t color = lv_palette_main(value < 50 ? LV_PALETTE_RED : LV_PALETTE_GREEN);
    lv_obj_set_style_text_color(label, color, 0);
}

// �������鶯��
static void start_slider_animation(void)
{
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, slider);
    lv_anim_set_exec_cb(&a, slider_anim);
    lv_anim_set_values(&a, 0, 100);
    lv_anim_set_time(&a, 2000);
    lv_anim_set_playback_time(&a, 2000);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&a);
}

// ���ӵ�LVGL���Ժ���
void lv_LVGL_Test(void)
{
    // ������Ļ
    lv_obj_t * scr = lv_scr_act();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x101010), 0);
    
    // ���������ǩ
    lv_obj_t * title = lv_label_create(scr);
    lv_label_set_text(title, "LVGL Complex Demo");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);
    
    // ����ͼ��
    chart = lv_chart_create(scr);
    lv_obj_set_size(chart, 200, 120);
    lv_obj_align(chart, LV_ALIGN_TOP_LEFT, 10, 50);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(chart, 20);
    ser1 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    
    // �������ν�����
    arc = lv_arc_create(scr);
    lv_obj_set_size(arc, 100, 100);
    lv_arc_set_rotation(arc, 270);
    lv_arc_set_bg_angles(arc, 0, 360);
    lv_arc_set_value(arc, 0);
    lv_obj_align(arc, LV_ALIGN_TOP_RIGHT, -20, 60);
    
    // �������α�ǩ
    label = lv_label_create(scr);
    lv_label_set_text(label, "0%");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_16, 0);
    lv_obj_align_to(label, arc, LV_ALIGN_CENTER, 0, 0);
    
    // ��������
    slider = lv_slider_create(scr);
    lv_obj_set_width(slider, 150);
    lv_obj_align(slider, LV_ALIGN_BOTTOM_MID, 0, -50);
    lv_slider_set_range(slider, 0, 100);
    lv_slider_set_value(slider, 50, LV_ANIM_OFF);
    
    // ����������
    bar = lv_bar_create(scr);
    lv_obj_set_size(bar, 150, 20);
    lv_obj_align_to(bar, slider, LV_ALIGN_OUT_TOP_MID, 0, -10);
    lv_bar_set_range(bar, 0, 100);
    
    // ����������ǩ
    lv_obj_t * anim_label = lv_label_create(scr);
    lv_label_set_text(anim_label, "Animated Label");
    lv_obj_set_style_text_font(anim_label, &lv_font_montserrat_14, 0);
    lv_obj_align(anim_label, LV_ALIGN_BOTTOM_LEFT, 10, -10);
    
    // ������ǩ��ɫ�仯����
    lv_anim_t label_anim;
    lv_anim_init(&label_anim);
    lv_anim_set_var(&label_anim, anim_label);
    lv_anim_set_values(&label_anim, 0, 360);
    lv_anim_set_exec_cb(&label_anim, (lv_anim_exec_xcb_t)lv_obj_set_style_bg_color);
    lv_anim_set_time(&label_anim, 3000);
    lv_anim_set_repeat_count(&label_anim, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&label_anim);
    
    // �������鶯��
    start_slider_animation();
    
    // ������ʱ������ͼ��
    lv_timer_create(update_chart_timer, 300, NULL);
    
    // ������ʱ�����»��ν�����
    lv_timer_create(arc_loader, 50, NULL);
    
    // ��������������
    lv_anim_t bar_anim;
    lv_anim_init(&bar_anim);
    lv_anim_set_var(&bar_anim, bar);
    lv_anim_set_values(&bar_anim, 0, 100);
    lv_anim_set_exec_cb(&bar_anim, (lv_anim_exec_xcb_t)lv_bar_set_value);
    lv_anim_set_time(&bar_anim, 1500);
    lv_anim_set_playback_time(&bar_anim, 1500);
    lv_anim_set_repeat_count(&bar_anim, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&bar_anim);
}


void Task_LVGL_Tick(void *argument)
{
    // lv_LVGL_Test();
	lv_demo_stress();
	while(1)
	{
		lv_timer_handler();
		osDelay(5);
	}
}