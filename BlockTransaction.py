import zipfile

fileDir = "./"    
files = [
    "20000000to20249999_BlockTransaction"
]

def ToInt(str):
    return None if str=="None" else int(str)
def ToStr(str):
    return None if str=="None" else str

tx_count = 0
total_fees = 0
total_blobs = 0

for file in files:
    print(file)    
    theZIP = zipfile.ZipFile(fileDir+file+".zip", 'r')
    theCSV = theZIP.open(file+".csv")
    head = theCSV.readline()

    oneLine = theCSV.readline().decode("utf-8").strip()    
    while (oneLine!=""):
        oneArray = oneLine.split(",")
        blockNumber           = int(oneArray[0])
        timestamp             = int(oneArray[1])
        transactionHash       = oneArray[2]
        sender                = oneArray[3]
        to                    = oneArray[4]
        toCreate              = oneArray[5]
        fromIsContract        = oneArray[6]
        toIsContract          = oneArray[7]
        value                 = int(oneArray[8])
        gasLimit              = int(oneArray[9])
        gasPrice              = int(oneArray[10])
        gasUsed               = int(oneArray[11])
        callingFunction       = oneArray[12]
        isError               = ToStr(oneArray[13])
        eip2718type           = ToInt(oneArray[14])
        baseFeePerGas         = ToInt(oneArray[15])
        maxFeePerGas          = ToInt(oneArray[16])
        maxPriorityFeePerGas  = ToInt(oneArray[17])

        if eip2718type == 3:
            blobHashes        = oneArray[18].split(":")
            blobBaseFeePerGas = int(oneArray[19])
            blobGasUsed       = int(oneArray[20])

            total_blobs += len(blobHashes)
            total_fees  += blobBaseFeePerGas*blobGasUsed


        total_fees += gasPrice*gasUsed
        tx_count += 1    
        if tx_count % 100000 == 0:
            print(tx_count, total_fees/1e+18, total_blobs)    
        oneLine = theCSV.readline().decode("utf-8").strip()    

    theCSV.close()    
    theZIP.close()    


print(tx_count, total_fees/1e+18, total_blobs)
