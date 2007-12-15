class RestoreWindow : public QDialog, private Ui::RestoreWindow
{
    Q_OBJECT
    
public:
    RestoreWindow() setupUi(this);

private:
	void loadList();
	
	QStringList synchronised;
};

RestoreWindow::RestoreWindow()
{
	setupUi(this);
	
	synchronised = window->synchronised;	
	loadList();
	
	
}

void RestoreWindow::loadList()
{
 	for (int i = 0; i < synchronised.count(); ++i) {
         if (i % 4 == 0 || i == 0) {
		 	   restoreList-> recentsyncs.at(i);
				}
     }
}
