/* Copyright (c) 2020-2022 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "../global.h"
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include "consoleoutput.h"
#include "die_script.h"
#include "entropyprocess.h"
#include "xoptions.h"
#include "xfileinfo.h"

void ScanFiles(QList<QString> *pListArgs,DiE_Script::OPTIONS *pScanOptions,DiE_Script *pDieScript)
{
    QList<QString> listFileNames;

    for(qint32 i=0;i<pListArgs->count();i++)
    {
        QString sFileName=pListArgs->at(i);

        if(QFileInfo::exists(sFileName))
        {
            XBinary::findFiles(sFileName,&listFileNames);
        }
        else
        {
            printf("Cannot find: %s\n",sFileName.toUtf8().data());
        }
    }

    bool bShowFileName=listFileNames.count()>1;

    qint32 nNumberOfFiles=listFileNames.count();

    for(qint32 i=0;i<nNumberOfFiles;i++)
    {
        QString sFileName=listFileNames.at(i);

        if(bShowFileName)
        {
            printf("%s:\n",QDir().toNativeSeparators(sFileName).toUtf8().data());
        }

        if(pScanOptions->bShowEntropy)
        {
            QString sResult;

            EntropyProcess::DATA epData=EntropyProcess::processRegionsFile(sFileName);

            if(pScanOptions->bResultAsJSON)
            {
                sResult=EntropyProcess::dataToJsonString(&epData);
            }
            else if(pScanOptions->bResultAsXML)
            {
                sResult=EntropyProcess::dataToXmlString(&epData);
            }
            else if(pScanOptions->bResultAsCSV)
            {
                sResult=EntropyProcess::dataToCsvString(&epData);
            }
            else if(pScanOptions->bResultAsTSV)
            {
                sResult=EntropyProcess::dataToTsvString(&epData);
            }
            else
            {
                sResult=EntropyProcess::dataToPlainString(&epData);
            }

            printf("%s",sResult.toUtf8().data());
        }
        else if((pScanOptions->bShowExtraInfo)||(pScanOptions->sSpecial!=""))
        {
            QString sResult;

            XFileInfo::OPTIONS options={};

            if(pScanOptions->sSpecial!="")
            {
                options.sString=pScanOptions->sSpecial;
            }
            else
            {
                options.sString="Info";
            }

            XFileInfoModel model;

            XFileInfo::processFile(sFileName,&model,options);

            if(pScanOptions->bResultAsJSON)
            {
                sResult=model.toJSON();
            }
            else if(pScanOptions->bResultAsXML)
            {
                sResult=model.toXML();
            }
            else if(pScanOptions->bResultAsCSV)
            {
                sResult=model.toCSV();
            }
            else if(pScanOptions->bResultAsTSV)
            {
                sResult=model.toTSV();
            }
            else
            {
                sResult=model.toFormattedString();
            }

            printf("%s",sResult.toUtf8().data());
            printf("\n");
        }
        else
        {
            DiE_Script::SCAN_RESULT scanResult=pDieScript->scanFile(sFileName,pScanOptions);

            QList<XBinary::SCANSTRUCT> listResult=DiE_Script::convert(&(scanResult.listRecords));

            ScanItemModel model(&listResult);

            XBinary::FORMATTYPE formatType=XBinary::FORMATTYPE_TEXT;

            if      (pScanOptions->bResultAsCSV)    formatType=XBinary::FORMATTYPE_CSV;
            else if (pScanOptions->bResultAsJSON)   formatType=XBinary::FORMATTYPE_JSON;
            else if (pScanOptions->bResultAsTSV)    formatType=XBinary::FORMATTYPE_TSV;
            else if (pScanOptions->bResultAsXML)    formatType=XBinary::FORMATTYPE_XML;

            if(formatType!=XBinary::FORMATTYPE_TEXT)
            {
                printf("%s\n",model.toString(formatType).toUtf8().data());
            }
            else
            {
                // Colored text
                model.coloredOutput();
            }

//            QList<XBinary::SCANSTRUCT> listResult=DiE_Script::convert(&(scanResult.listRecords));

//            ScanItemModel model(&listResult);

//            printf("%s",model.toFormattedString().toUtf8().data());

            if(scanResult.listErrors.count())
            {
                printf("%s",DiE_Script::getErrorsString(&scanResult).toUtf8().data());
            }
            printf("\n");
        }
    }
}

int main(int argc,char *argv[])
{
    QCoreApplication::setOrganizationName(X_ORGANIZATIONNAME);
    QCoreApplication::setOrganizationDomain(X_ORGANIZATIONDOMAIN);
    QCoreApplication::setApplicationName(X_APPLICATIONNAME);
    QCoreApplication::setApplicationVersion(X_APPLICATIONVERSION);

    QCoreApplication app(argc,argv);

    QCommandLineParser parser;
    QString sDescription;
    sDescription.append(QString("%1 v%2\n").arg(X_APPLICATIONDISPLAYNAME,X_APPLICATIONVERSION));
    sDescription.append(QString("%1\n").arg("Copyright(C) 2006-2008 Hellsp@wn 2012-2022 hors<horsicq@gmail.com> Web: http://ntinfo.biz"));
    parser.setApplicationDescription(sDescription);
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("target","The file or directory to open.");

    QCommandLineOption clRecursiveScan  (QStringList()<<    "r"<<   "recursivescan",    "Recursive scan."       );
    QCommandLineOption clDeepScan       (QStringList()<<    "d"<<   "deepscan",         "Deep scan."            );
    QCommandLineOption clHeuristicScan  (QStringList()<<    "u"<<   "heuristicscan",    "Heuristic scan."       );
    QCommandLineOption clVerbose        (QStringList()<<    "b"<<   "verbose",          "Verbose."              );
    QCommandLineOption clAllTypesScan   (QStringList()<<    "a"<<   "alltypes",         "Scan all types."       );
    QCommandLineOption clEntropy        (QStringList()<<    "e"<<   "entropy",          "Show entropy."         );
    QCommandLineOption clInfo           (QStringList()<<    "i"<<   "info",             "Show file info."       );
    QCommandLineOption clResultAsXml    (QStringList()<<    "x"<<   "xml",              "Result as XML."        );
    QCommandLineOption clResultAsJson   (QStringList()<<    "j"<<   "json",             "Result as JSON."       );
    QCommandLineOption clResultAsCSV    (QStringList()<<    "c"<<   "csv",              "Result as CSV."        );
    QCommandLineOption clResultAsTSV    (QStringList()<<    "t"<<   "tsv",              "Result as TSV."        );
    QCommandLineOption clDatabase       (QStringList()<<    "D"<<   "database",         "Set database<path>.",      "path");
    QCommandLineOption clShowDatabase   (QStringList()<<    "s"<<   "showdatabase",     "Show database."        );
    QCommandLineOption clSpecial        (QStringList()<<    "S"<<   "special",          "Special file info for <method>. For example -S \"Hash\".",   "method");
    QCommandLineOption clShowMethods    (QStringList()<<    "m"<<   "showmethods",      "Show all special methods for the file.");

    parser.addOption(clRecursiveScan);
    parser.addOption(clDeepScan);
    parser.addOption(clHeuristicScan);
    parser.addOption(clVerbose);
    parser.addOption(clAllTypesScan);
    parser.addOption(clEntropy);
    parser.addOption(clInfo);
    parser.addOption(clSpecial);
    parser.addOption(clResultAsXml);
    parser.addOption(clResultAsJson);
    parser.addOption(clResultAsCSV);
    parser.addOption(clResultAsTSV);
    parser.addOption(clDatabase);
    parser.addOption(clShowDatabase);
    parser.addOption(clShowMethods);

    parser.process(app);

    QList<QString> listArgs=parser.positionalArguments();

    DiE_Script::OPTIONS scanOptions={0};

    scanOptions.bShowType=true;
    scanOptions.bShowOptions=true;
    scanOptions.bShowVersion=true;
    scanOptions.bRecursiveScan=parser.isSet(clRecursiveScan);
    scanOptions.bIsDeepScan=parser.isSet(clDeepScan);
    scanOptions.bIsHeuristicScan=parser.isSet(clHeuristicScan);
    scanOptions.bIsVerbose=parser.isSet(clVerbose);
    scanOptions.bAllTypesScan=parser.isSet(clAllTypesScan);
    scanOptions.bShowEntropy=parser.isSet(clEntropy);
    scanOptions.bShowExtraInfo=parser.isSet(clInfo);
    scanOptions.bResultAsXML=parser.isSet(clResultAsXml);
    scanOptions.bResultAsJSON=parser.isSet(clResultAsJson);
    scanOptions.bResultAsCSV=parser.isSet(clResultAsCSV);
    scanOptions.bResultAsTSV=parser.isSet(clResultAsTSV);

    scanOptions.sSpecial=parser.value(clSpecial);

    QString sDatabase=parser.value(clDatabase);

    if(sDatabase=="")
    {
        sDatabase=XOptions().getApplicationDataPath()+QDir::separator()+"db";
    }

    ConsoleOutput consoleOutput;
    DiE_Script die_script;

    QObject::connect(&die_script,SIGNAL(errorMessage(QString)),&consoleOutput,SLOT(errorMessage(QString)));
    QObject::connect(&die_script,SIGNAL(infoMessage(QString)),&consoleOutput,SLOT(infoMessage(QString)));

    bool bIsDbUsed=false;

    if(parser.isSet(clShowDatabase))
    {
        if(!bIsDbUsed)
        {
            die_script.loadDatabase(sDatabase);
            bIsDbUsed=true;
        }

        printf("Database: %s\n",sDatabase.toUtf8().data());

        QList<DiE_Script::SIGNATURE_STATE> list=die_script.getSignatureStates();

        qint32 nNumberOfRecords=list.count();

        for(qint32 i=0;i<nNumberOfRecords;i++)
        {
            printf("\t%s: %d\n",XBinary::fileTypeIdToString(list.at(i).fileType).toUtf8().data(),list.at(i).nNumberOfSignatures);
        }
    }

    if(parser.isSet(clShowMethods))
    {
        XBinary::FT fileType=XBinary::FT_UNKNOWN;

        if(listArgs.count())
        {
            fileType=XBinary::getPrefFileType(listArgs.at(0));
        }

        printf("Methods:\n");

        QList<XFileInfo::METHOD> listMethods=XFileInfo::getMethodNames(fileType);

        qint32 nNumberOfMethods=listMethods.count();

        for(qint32 i=0;i<nNumberOfMethods;i++)
        {
            printf("\t%s\n",listMethods.at(i).sName.toUtf8().data());
        }
    }
    else if(listArgs.count())
    {
        if(!bIsDbUsed)
        {
            die_script.loadDatabase(sDatabase);
            bIsDbUsed=true;
        }

        ScanFiles(&listArgs,&scanOptions,&die_script);
    }
    else if(!parser.isSet(clShowDatabase))
    {
        parser.showHelp();
        Q_UNREACHABLE();
    }

    return 0;
}
