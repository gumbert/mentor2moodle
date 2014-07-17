
#include <QtCore/QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QString>
#include <QStringList>
QString imToBase64(QString);
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	system("chcp 1251");
	const QStringList& arguments=QCoreApplication::arguments();
	
 
    // ������������� ��������� � ������ ����� �������� ��������� (UTF-8)
    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
	//QTextCodec::setCodecForLocale(QTextCodec::codecForName("IBM866"));
	
    QString inNum,inAnsw;
	QString Category="Opred";			//��������� ��������
	int task,taskMax=4;					// ����� ������� ������
	int variant	, variantMax=4;			//����� ��������
	int AnswerCount=5;					//���������� ������� � �����	
	
	QString AB="0 a b c d e f g h i j k l m n o p r s t u v w x y z";
	QStringList ABC =AB.split(' ');
	
	QString path;
	
	if(arguments.count()>1)
	{
		if(arguments.at(1)=="-d")
		{
			path=arguments.at(2);
		}
		if(arguments.at(1)=="-h")
		{
			printf("��������� ��� �������� ������ ��� Moodle �� ������ \n������ ��������������� Mentor2.");
			printf("help\n��� ���� ����� �������� ���������� ������ �������� ������\n ���������� ��������� ��������� � ���������� -d \n � ����� ������� ���� �� ������� ����������.");
			return 0;
		}
	}

	//���������� ���������� � �����
	
	if(!path.isEmpty())
	{
		path.append('\\');
	}

	QFile infNum(path+"Number.txt");
	QFile infAnsw(path+"right.txt");

	if(infNum.open(QIODevice::ReadOnly))
    {
        QTextStream inN(&infNum);
        inN.setCodec("cp1251");          // ��������� ������
        inNum = inN.readAll();       // ��������� ���� ���� � QString
    }
	else 
	{
		printf("���� Number.txt �� ��� ������. ��������� ��� � ������������� ���������.");
		return 0;
	}
	infNum.close();
	QStringList inf=inNum.split('\n');
	taskMax=inf[3].toInt();
	variantMax=inf[2].toInt();
	Category = inf[1];
	
	QFile outfXML(path+"toMoodle.xml");
	//���������� ���������� �������
	if(infAnsw.open(QIODevice::ReadOnly))
    {
        QTextStream inA(&infAnsw);
        inA.setCodec("UTF-8");          // ��������� ������
        inAnsw = inA.readAll();       // ��������� ���� ���� � QString
    }
	else
	{
		printf("���� right.txt �� ��� ������. ��������� ��� � ������������� ���������.");
		return 0;
	}
    infAnsw.close();
	QStringList Answers=inAnsw.split('\n');

    if(outfXML.open(QIODevice::ReadWrite))
    {
        QTextStream out(&outfXML);
		out.setCodec("UTF-8");          // ��������� ������
		
		//���������� XML ����
		out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<quiz>";
		for(task=1;task<=taskMax;task++)
		{
			out <<"\n\t<question type=\"category\">\n\t\t<category>\n\t\t\t<text>$course$/";
			out << Category;
			QString que2=QObject::tr("/������ ");
			out << que2;
			out << QString::number(task);
			out << "</text>\n\t\t</category>\n\t</question>\n";

			for(variant=1;variant<=variantMax;variant++)
			{
				out<<"\n\t<question type=\"multichoice\">\n\t\t<name>\n\t\t\t<text>";
				QString que=QObject::tr("�������  ");
				out<< que;
				out<<QString::number((task-1)*variantMax+variant);
				out<<"</text>\n\t\t</name>\n\t\t<questiontext format=\"html\">\n\t\t\t<text>";
				out<< "\n\t\t<![CDATA[<img src=\" data:image/jpeg;base64,";
				
				QString ImName;
				ImName = path;
				ImName += "Image_";
				ImName += QString::number(variant);
				ImName += "_zad_";
				ImName += QString::number(task);
				ImName += ".jpg";
				QString Image=imToBase64(ImName);
				
				out << Image;
				out << "\">]]>\n\t\t\t</text>\n\t\t";
				out << "</questiontext>";
			
				for (int i=1;i<=AnswerCount;i++)
				{
					if(i==Answers[taskMax*(variant-1)+task-1].toInt())
					{
						out<<"\n\t\t<answer fraction=\"100\">\n\t\t\t<text>";
						out<<ABC[i];
						QString que3=QObject::tr("</text>\n\t\t\t<feedback><text>���������!</text></feedback>\n\t\t</answer>");
						out << que3;
					}
					else
					{
						out<<"\n\t\t<answer fraction=\"0\">\n\t\t\t<text>";
						out<<ABC[i];
						QString que4=QObject::tr("</text>\n\t\t\t<feedback><text>������!</text></feedback>\n\t\t</answer>");
						out << que4;
					}
				}

				out << "<shuffleanswers>1</shuffleanswers>\n\t\t<single>true</single>\n\t\t<answernumbering>123</answernumbering>\n\t</question>";

			}
		}
		out << "</quiz>";
		outfXML.close();
		printf("\n\n\n������.");
    }
 
    
    return 0;
}
QString imToBase64(QString name)
{
	QFile* file = new QFile(name);
	file->open(QIODevice::ReadOnly);
	QByteArray image = file->readAll();
	int originalSize = image.length();

	QString encoded = QString(image.toBase64());
	file->close();
	return encoded;
}