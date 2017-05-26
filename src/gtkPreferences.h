/*
** gtkPreferences.h - The dialog for collecting preference information.
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
*/

#define GTKPREF_DECLARE(name,vartype) 					\
public: 								\
	virtual vartype Get##name() const {return m##name;} 		\
	virtual void Set##name(vartype s) {m##name = s;}		\
protected:								\
	static GtkWidget *m##name##Widget;	\
	static GtkWidget *m##name##Hbox;  	\
	static vartype m##name;				\

#define GTKPREF_DECLARE_STRING(name) 					\
public: 								\
	virtual char * Get##name() const {return m##name;} 		\
	virtual void Set##name(char * s) {m##name = new char[::strlen(s)]; ::strcpy(m##name,(char *)s);}		\
protected:								\
	static GtkWidget *m##name##Widget;	\
	static GtkWidget *m##name##Hbox;  	\
	static char * m##name;				\

#define GTKPREF_DEFINE(name,vartype) 			\
	GtkWidget * GtkPreferences::m##name##Widget = NULL;		\
	GtkWidget * GtkPreferences::m##name##Hbox = NULL;		\
	vartype GtkPreferences::m##name = 0;				\


#define GTKPREF_ADD_ENTRY_WIDGET(name,vartype,porder,parent,plabel,printmask) \
        GtkWidget *hbox##porder = gtk_hbox_new (FALSE, 0);		\
        m##name##Hbox = hbox##porder;  \
        gtk_container_add (GTK_CONTAINER (parent), hbox##porder);	\
        gtk_container_set_border_width (GTK_CONTAINER (hbox##porder), 5);\
        gtk_widget_show (hbox##porder);					\
									\
        label = gtk_label_new (#plabel);				\
        gtk_box_pack_start (GTK_BOX (hbox##porder), label, TRUE,FALSE, 0);\
        gtk_widget_show (label);					\
									\
        m##name##Widget = gtk_entry_new ();				\
        gtk_editable_select_region (GTK_EDITABLE (m##name##Widget), 0, -1); \
        gtk_box_pack_start(GTK_BOX(hbox##porder),m##name##Widget,FALSE,FALSE,20);\
        char minline##porder[10];					\
        sprintf(minline##porder, #printmask, m##name);			\
        gtk_entry_set_text (GTK_ENTRY (m##name##Widget), minline##porder);\
        gtk_widget_show (m##name##Widget);		\

#define GTKPREF_ADD_CHECK_WIDGET(name,vartype,porder,parent,plabel,toggleCB) \
        GtkWidget *hbox##porder = gtk_hbox_new (FALSE, 0);		\
        gtk_container_add (GTK_CONTAINER (parent), hbox##porder);	\
        gtk_container_set_border_width (GTK_CONTAINER (hbox##porder), 5);\
        m##name##Hbox = hbox##porder;  \
        gtk_widget_show (hbox##porder);					\
									\
		m##name##Widget = gtk_check_button_new_with_label (#plabel);				\
        gtk_box_pack_start(GTK_BOX (hbox##porder),m##name##Widget,TRUE,FALSE,0);\
 		gtk_signal_connect (GTK_OBJECT (m##name##Widget), "clicked",\
                             GTK_SIGNAL_FUNC (##toggleCB), NULL);\
		gtk_widget_show (m##name##Widget);		\
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(m##name##Widget),mAutoColor);	\
		
 /*       label = gtk_label_new (#plabel);				\
        gtk_box_pack_start (GTK_BOX (hbox##porder), label, FALSE,FALSE, 0);\
        gtk_widget_show (label);					\
*/
#ifndef _GTKPREFERENCES_H_
#define _GTKPREFERENCES_H_

#include "preferences.h"
#include "rcPersistence.h"   //@REX this is disappointing -- see below
#include <gtk/gtk.h>


class GtkPreferences : public ContourPreferences
{
	friend class GtkApp;  //so he can delete our mInstance when he goes away
public:
	GtkPreferences();
        virtual ~GtkPreferences();
	bool Edit();  //return true if they changed. 
	static GtkPreferences *GetInstance();

	//@REX disappointing! Why can't I use the abstract base class here?
	// for some reason I am needing to cast to the concrete subclass.
	virtual void Save() {if (mPersistor) ((RCPersistence *)mPersistor)->Write();}

	virtual SamplePoint & GetUpperLeft() const;
	virtual void SetUpperLeft(const SamplePoint &sp);

	GTKPREF_DECLARE (MaxDepth,float)
	GTKPREF_DECLARE (MinDepth,float)
	GTKPREF_DECLARE (AutoColor,bool)
	GTKPREF_DECLARE (ShowGrid,bool)
	GTKPREF_DECLARE (Draw,bool)
	GTKPREF_DECLARE (AutoCenter,bool)
	GTKPREF_DECLARE (Track,bool)
	
	GTKPREF_DECLARE (XScale,float)
	GTKPREF_DECLARE (YScale,float)

	//postgres stuff
	GTKPREF_DECLARE_STRING(pgHost)
	GTKPREF_DECLARE_STRING(pgPort)
	GTKPREF_DECLARE_STRING(pgOptions)
	GTKPREF_DECLARE_STRING(pgDBName)
	GTKPREF_DECLARE_STRING(pgUser)
	GTKPREF_DECLARE_STRING(pgPassword)

	//gps
	GTKPREF_DECLARE_STRING(gpsHost)
	GTKPREF_DECLARE_STRING(gpsPort)
	GTKPREF_DECLARE(gpsSampleRate,int)
	
protected:
	static GtkPreferences *mInstance;
	//controls
	static GtkWidget *mDialog;
	static GtkWidget *mNotebook;
	static GtkWidget *mOKbutton;
	static GtkWidget *mCancelButton;

	static bool mOK;  //half-assed way of returning what was clicked(FIXME?)

	//pages
	void AddDepthPage();
	bool ReadDepthPage();
	void AddPostgresPage();
	bool ReadPostgresPage();
	void AddGPSPage();
	bool ReadGPSPage();
	void AddAdvancedPage();  //deprecated
	
	//SCREEN
	//no page yet
	static SamplePoint *mUpperLeft;

	//callbacks
	static void OnCancel(GtkWidget *widget, GtkFileSelection *fs);
	static void OnOK(GtkWidget *widget, GtkWidget *entry);
	static gint OnToggleAutoColor(GtkWidget *widget, GdkEvent *event, gpointer data);

};

#endif

