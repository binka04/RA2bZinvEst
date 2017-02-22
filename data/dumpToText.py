from ROOT import *
from array import array

def dumpFromTGraphAsymmError(inputFileName,targetObjectName,outputFileName):
    inputFile = TFile(inputFileName)
    targetObject = inputFile.Get(targetObjectName)
    
    x=Double(0.)
    y=Double(0.)
    outputFile = open(outputFileName,'w')
    for i in range(targetObject.GetN()):
        targetObject.GetPoint(i,x,y)
        #print i,y,targetObject.GetErrorYhigh(i),targetObject.GetErrorYlow(i)
        outputFile.write("{0} {1} {2} {3}\n".format(i,y,targetObject.GetErrorYhigh(i),targetObject.GetErrorYlow(i)))

    outputFile.close()

def dumpFromTH1(inputFileName,targetObjectName,outputFileName):
    inputFile = TFile(inputFileName)
    targetObject = inputFile.Get(targetObjectName)

    for i in range(targetObject.GetNbinsX()):
        outputFile.write("{0} {1} {1}\n".format(targetObject.GetBinContent(i+1),targetObject.GetBinError(i+1)))

    outputFile.close()

from optparse import OptionParser

parser = OptionParser()
parser.add_option("-f", "--file", dest="filename",
                  help="name of input file")
parser.add_option("-n", "--objectName",dest="object",
                  help="name of object that you want to dump")
parser.add_option("-o", "--output",dest="output",
                  help="name of output file name")

(options, args) = parser.parse_args()

dumpFromTGraphAsymmError(options.filename,options.object,options.output)
