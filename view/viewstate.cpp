#include "viewstate.h"

ViewState::ViewState()
{

}

ViewState::ViewState(Reciver *reciver,Region region)
{
    this->reciver=reciver;
    this->region=region;

    state=reciver->getState(region);
    tab=new QTabWidget;

    foreach (auto x, state.xctrls) {
        tab->addTab(new ViewXctrl(reciver,region,x),"XT:"+region.toKey()+":"+x->name);
    }
    QGridLayout *maingrid=new QGridLayout(this);
    maingrid->addWidget(tab,0,1);
    setLayout(maingrid);

}

void ViewState::Update()
{
    for (int i = 0; i < tab->count(); ++i) {
        if (tab->tabText(i).contains("XT:")){
            static_cast<ViewXctrl*>(tab->widget(i))->Update();
            continue;
        }
    }
    update();
}
