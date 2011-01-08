/*
 * Xournal++
 *
 * The main Control
 *
 * @author Xournal Team
 * http://xournal.sf.net
 *
 * @license GPL
 */

#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "RecentManager.h"
#include "../gui/MainWindow.h"
#include "Actions.h"
#include "../control/UndoRedoHandler.h"
#include "../control/BackgroundThreadHandler.h"
#include "Settings.h"
#include "ToolHandler.h"
#include "../model/Document.h"
#include "ZoomControl.h"

#include "../gui/sidebar/Sidebar.h"
#include "../gui/SearchBar.h"
#include "Cursor.h"

class Sidebar;
class Cursor;
class CallbackData;

class Control: public ActionHandler,
		public ToolListener,
		public DocumentHandler,
		public RecentManagerListener,
		public UndoRedoListener {
public:
	Control();
	virtual ~Control();

	void initWindow(MainWindow * win);
public:
	// Menu File
	void newFile();
	bool openFile(String filename = NULL);
	bool annotatePdf(String filename, bool attachPdf);
	void print();
	void exportAsPdf();
	void exportAs();
	bool save();
	void saveAs();
	void quit();
	bool close();

	// Menu edit
	void showSettings();

	// Menu Help
	void showAbout();

	Settings * getSettings();
	ToolHandler * getToolHandler();

	virtual void actionPerformed(ActionType type, ActionGroup group, GdkEvent *event, GtkMenuItem *menuitem,
			GtkToolButton *toolbutton, bool enabled);

	virtual void toolColorChanged();
	virtual void setCustomColorSelected();
	virtual void toolChanged();
	virtual void toolSizeChanged();
	virtual void eraserTypeChanged();

	void setEraserType(EraserType eraserType);

	void selectTool(ToolType type);

	void gotoAnnotatedPage(bool next);

	void updatePageNumbers(int page, int pdfPage);

	Document * getDocument();

	virtual void fileOpened(const char * uri);

	ZoomControl * getZoomControl();

	/**
	 * Save current state
	 */
	void saveSettings();

	void scrollToPage(XojPage * page, double top = 0);
	void scrollToPage(int page, double top = 0);

	UndoRedoHandler * getUndoRedoHandler();

	MainWindow * getWindow();

	Cursor * getCursor();

	void scrollRelative(double x, double y);
	void scrollToSpinPange();

	int getCurrentPageNo();

	XojPage * getCurrentPage();
	void calcZoomFitSize();

	void setViewTwoPages(bool continous);

	void setPageInsertType(PageInsertType type);

	bool copy(String source, String target);

	void enableFullscreen(bool enabled, bool presentation = false);

	void addNewLayer();
	void deleteCurrentLayer();

	void changePageBackgroundColor();
	void setPageBackground(ActionType type);
	void updateBackgroundSizeButton();

	bool isFullscreen();

	static String getFilename(String uri);

	bool searchTextOnPage(const char * text, int p, int * occures, double * top);

	void goToPreviousPage();
	void goToNextPage();

	void firePageSelected(XojPage * page);
	void firePageSelected(int page);

	void addDefaultPage();
	void insertNewPage(int position);
	void deletePage();

	/**
	 * Disable / enable delete page button
	 */
	void updateDeletePageButton();

	void runInBackground(Runnable * runnable);

public:
	// UndoRedoListener interface
	void undoRedoChanged();
	void undoRedoPageChanged(XojPage * page);

protected:
	String showOpenDialog(bool pdf, bool & attachPdf);
	static bool invokeCallback(CallbackData * cb);
	void invokeLater(ActionType type);
	void zoomFit();

	bool shouldIgnorAction(ActionType type, ActionGroup group, bool enabled);

	bool showSaveDialog();

	void updateDocName();
	void fileLoaded();

	void eraserSizeChanged();
	void penSizeChanged();
	void hilighterSizeChanged();

	static void copyProgressCallback(goffset current_num_bytes, goffset total_num_bytes, Control * control);

	static bool checkChangedDocument(Control * control);

	void updatePreview();
private:
	RecentManager * recent;
	UndoRedoHandler * undoRedo;
	ZoomControl * zoom;
	bool fullscreen;

	Settings * settings;
	MainWindow * win;

	Document * doc;

	Sidebar * sidebar;
	SearchBar * searchBar;

	ToolHandler * toolHandler;

	Cursor * cursor;

	ActionType lastAction;
	ActionGroup lastGroup;
	bool lastEnabled;

	String copyError;

	GList * hiddenFullscreenWidgets;
	bool sidebarHidden;

	BackgroundThreadHandler * background;

	/**
	 * Timeout id: the timeout watches the changes and actualizes the previews from time to time
	 */
	int changeTimout;

	/**
	 * The pages wihch has changed since the last update (for preview update)
	 */
	GList * changedPages;
};

class CallbackData {
public:
	CallbackData(Control * control, ActionType type) {
		this->control = control;
		this->type = type;
	}

	ActionType type;
	Control * control;
};

#endif /* __CONTROL_H__ */