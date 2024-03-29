#include "pltarget.h"
#include "gdefine.h"
#include "vnet.h"
#include "vtcp.h"
#include "cfgbus.h"
#include "k_command.h"

static char cmdsbuf[MAXFRMDATASIZE];

//定时器中刷新各个引脚和变量的值，并更新画面
void PLTarget::timerEvent(QTimerEvent *e)
{
    if(0 == cfgbus_send(FUNCPRJINFO, 0, NULL)){
        prjinfo_t *pinfo = cfgbus_prjinfo();
        idCmdTarget = pinfo->id_cmd;
        uuidTarget = pinfo->uuid;
        //qDebug() << uuidTarget << idCmdTarget;
    }else{
        online(false, NULL);
    }

    if(bMonitor){        
        if(0 == cfgbus_send(FUNCIMG, 0, NULL)){
            int i, j, k, n;
            // load fbs value
            PLProgram *prg;
            PLFunctionBlock *fb;
            fb_t *p_fb;
            for(i=0; i<gMainModel->prgList.size(); i++){
                prg = &gMainModel->prgList[i];
                for(j=0; j<prg->fbs.size(); j++){
                    fb = &prg->fbs[j];
                    p_fb = prj_fbfind(fb->idPrg, fb->id);
                    n = 0;
                    // input
                    for(k=0; k<fb->input.size(); k++){
                        fb->input[k].value.v = p_fb->d[n].v;
                        n++;
                    }
                    // output
                    for(k=0; k<fb->output.size(); k++){
                        fb->output[k].value.v = p_fb->d[n].v;
                        n++;
                        //qDebug() << "fb" << fb->id << "pin" << k << "val" << fb->output[k].value.v.i;
                    }
                    // property
                    for(k=0; k<fb->property.size(); k++){
                        fb->property[k].value.v = p_fb->d[n].v;
                        n++;
                    }
                }
            }
            // load evs value
            PLEVData *ev;
            val_t *p_ev;
            for(int i=0; i<gMainModel->evList.size(); i++){
                ev = &gMainModel->evList[i];
                p_ev = ev_find(ev->id);
                ev->value.v = p_ev->v;
            }
            gMainFrame->updateCadView();
        }else{
            online(false, NULL);
        }
    }

    //static int i = 0;
    //qDebug() << "timer:" << i++ << uuidTarget;
}

PLTarget::PLTarget(QObject *parent)
    : QTimer(parent)
{
    vnet_init();
    vtcp_init();
    cfgbus_init(0);

    bOnline = false;
    bMonitor = false;

    setInterval(200);
}

PLTarget::~PLTarget()
{

}

bool PLTarget::online(bool mode, char *ip)
{
    if(mode == bOnline){
        return false;
    }

    if(mode){
        if(0 == cfgbus_connect(ip)){
            bOnline = true;
            start();
        }
    }else{
        stop();
        cfgbus_disconnect();
        bOnline = false;
        bMonitor = false;
        idCmdTarget = 0;
        uuidTarget.clear();
        gMainFrame->updateCadView();
    }

    return bOnline;
}

bool PLTarget::sync()
{
    if(!bOnline){
        return false;
    }
    if(bMonitor){
        return false;
    }
    if(!isMatch()){
        return false;
    }
    if(isSync()){
        return false;
    }

    QString bigCmd;
    for(int i=0; i<gMainModel->cmdList.size(); i++){
        if(gMainModel->cmdList.at(i).id > idCmdTarget){
            bigCmd += gMainModel->cmdList.at(i).cmdLine;
            bigCmd += "\n";
        }
    }
    if(bigCmd.size() >= sizeof(cmdsbuf)){
        return false;
    }
    strncpy(cmdsbuf, bigCmd.toStdString().c_str(), sizeof(cmdsbuf)-1);
    cmdsbuf[sizeof(cmdsbuf)-1] = 0;

    stop();

    if(0 != cfgbus_send(FUNCSTOP, 0, NULL)){
        online(false, NULL);
        return false;
    }

    if(0 == cfgbus_send(FUNCCMD, strlen(cmdsbuf)+1, cmdsbuf)){
        cfgbus_send(FUNCRUN, 0, NULL);
        start();
        return true;
    }else{
        online(false, NULL);
        return false;
    }
}

void PLTarget::monitor(bool mode)
{
    if(mode){
        if(!bOnline){
            return;
        }
        if(!isSync()){
            return;
        }
        bMonitor = true;
    }else{
        bMonitor = false;
    }
}

bool PLTarget::download()
{
    if(!bOnline){
        return false;
    }
    if(bMonitor){
        return false;
    }
    if(isSync()){
        return false;
    }

    gMainModel->project.renewUuid();
    char cline[1024];
    strncpy(cline, gMainModel->project.getProjCmdLine().toStdString().c_str(), sizeof(cline)-1);
    cmd_dispatch(cline);

    QString bigCmd = gMainModel->project.getProjCmdLine();
    bigCmd += "\n";
    for(int i=0; i<gMainModel->cmdList.size(); i++){
        bigCmd += gMainModel->cmdList.at(i).cmdLine;
        bigCmd += "\n";
    }
    if(bigCmd.size() >= sizeof(cmdsbuf)){
        return false;
    }
    //qDebug() << bigCmd;
    strncpy(cmdsbuf, bigCmd.toStdString().c_str(), sizeof(cmdsbuf)-1);
    cmdsbuf[sizeof(cmdsbuf)-1] = 0;
    stop();

    if(0 != cfgbus_send(FUNCSTOP, 0, NULL)){
        //qDebug() << "Err: FUNCSTOP";
        online(false, NULL);
        return false;
    }
    if(0 != cfgbus_send(FUNCRESET, 0, NULL)){
        //qDebug() << "Err: FUNCRESET";
        online(false, NULL);
        return false;
    }

    if(0 == cfgbus_send(FUNCCMD, strlen(cmdsbuf)+1, cmdsbuf)){
        cfgbus_send(FUNCRUN, 0, NULL);
        start();
        return true;
    }else{
        //qDebug() << "Err: FUNCSTART";
        online(false, NULL);
        return false;
    }
}

bool PLTarget::upload(QString &cmds)
{
    if(0 != cfgbus_send(FUNCUCMD,0,NULL)){
        online(false, NULL);
        return false;
    }

    cmds = cfgbus_cmdsbuf();
    return true;
}

bool PLTarget::isOnline()
{
    return bOnline;
}

bool PLTarget::isMonitor()
{
    return bMonitor;
}

bool PLTarget::isMatch()
{
    if(uuidTarget == gMainModel->project.uuid){
        if(idCmdTarget <= gMainModel->cmdID){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

bool PLTarget::isSync()
{
    if(isMatch()){
        if(idCmdTarget == gMainModel->cmdID){
            return true;
        }
    }
    return false;
}
