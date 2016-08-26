#include "const.h"
#include "setting.h"

#include <QFile>
#include <QTextStream>
#include <QDomDocument>

bool Setting::EnableDragsAndDrops = true;
bool Setting::ShowWeekNumber = true;
int Setting::WeekFirstDay = 1;
int Setting::Language = 0;
int Setting::Opacity = 10;
int Setting::CellSpace = 2;
QFont Setting::InterfaceFont = QFont("微软雅黑", 9);
QFont Setting::EventFont = QFont("微软雅黑", 9);
QColor Setting::CellColor = Const::COLOR_LIST[14];
QColor Setting::ContinuousEventColor = Const::COLOR_LIST[11];
QColor Setting::RecurrentEventColor = Const::COLOR_LIST[10];

void Setting::LoadSetting(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QDomDocument doc;
    if (!doc.setContent(&file)) { file.close(); return; }

    QDomElement root = doc.documentElement();
    if (root.isNull() || root.tagName() != "Setting") { file.close(); return; }

    QDomNodeList list = root.childNodes();

    for (int i = 0; i < list.count(); i++)
    {
        QDomElement node = list.item(i).toElement();
        if (node.tagName() == "EnableDragsAndDrops" && node.hasAttribute("value"))
            EnableDragsAndDrops = node.attribute("value").toInt();

        if (node.tagName() == "ShowWeekNumber" && node.hasAttribute("value"))
            ShowWeekNumber = node.attribute("value").toInt();

        if (node.tagName() == "WeekFirstDay" && node.hasAttribute("value"))
            WeekFirstDay = node.attribute("value").toInt();

        if (node.tagName() == "Language" && node.hasAttribute("value"))
            Language = node.attribute("value").toInt();

        if (node.tagName() == "Opacity" && node.hasAttribute("value"))
            Opacity = node.attribute("value").toInt();

        if (node.tagName() == "CellSpace" && node.hasAttribute("value"))
            CellSpace = node.attribute("value").toInt();

        if (node.tagName() == "InterfaceFont" && node.hasAttribute("value"))
            InterfaceFont.fromString(node.attribute("value"));

        if (node.tagName() == "EventFont" && node.hasAttribute("value"))
            EventFont.fromString(node.attribute("value"));

        if (node.tagName() == "CellColor" && node.hasAttribute("value"))
            CellColor = QColor(node.attribute("value"));

        if (node.tagName() == "ContinuousEventColor" && node.hasAttribute("value"))
            ContinuousEventColor = QColor(node.attribute("value"));

        if (node.tagName() == "RecurrentEventColor" && node.hasAttribute("value"))
            RecurrentEventColor = QColor(node.attribute("value"));
    }
    file.close();
}

void Setting::SaveSetting(const QString &fileName)
{
    QDomDocument doc;
    QDomProcessingInstruction xml = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(xml);
    QDomElement root = doc.createElement("Setting");
    doc.appendChild(root);

    QDomElement node = doc.createElement("EnableDragsAndDrops");
    node.setAttribute("value", QString::number(EnableDragsAndDrops));
    root.appendChild(node);

    node = doc.createElement("ShowWeekNumber");
    node.setAttribute("value", QString::number(ShowWeekNumber));
    root.appendChild(node);

    node = doc.createElement("WeekFirstDay");
    node.setAttribute("value", QString::number(WeekFirstDay));
    root.appendChild(node);

    node = doc.createElement("Language");
    node.setAttribute("value", QString::number(Language));
    root.appendChild(node);

    node = doc.createElement("Opacity");
    node.setAttribute("value", QString::number(Opacity));
    root.appendChild(node);

    node = doc.createElement("CellSpace");
    node.setAttribute("value", QString::number(CellSpace));
    root.appendChild(node);

    node = doc.createElement("InterfaceFont");
    node.setAttribute("value", InterfaceFont.toString());
    root.appendChild(node);

    node = doc.createElement("EventFont");
    node.setAttribute("value", EventFont.toString());
    root.appendChild(node);

    node = doc.createElement("CellColor");
    node.setAttribute("value", CellColor.name());
    root.appendChild(node);

    node = doc.createElement("ContinuousEventColor");
    node.setAttribute("value", ContinuousEventColor.name());
    root.appendChild(node);

    node = doc.createElement("RecurrentEventColor");
    node.setAttribute("value", RecurrentEventColor.name());
    root.appendChild(node);

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream out(&file);
    doc.save(out, 2);
    file.close();
}
