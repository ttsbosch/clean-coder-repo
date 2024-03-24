#include "TradeProcessor.h"
void TradeProcessor::ProcessTrades(std::istream& stream)
    {
        
        std::vector<std::string> items;
        std::string item;
        while (std::getline(stream, item))
        {
            items.push_back(item);
        }

        std::vector<TradeRecord> records;
        int lineCount = 1;
        for (const auto& item : items)
        {
            std::vector<std::string> fields ;
          
            std::string t;
            std::istringstream tStream(item);
            while (std::getline(tStream, t, ','))
            {
            fields.push_back(t);
            }
            if (fields.size() != 3)
            {
                std::cout << "WARN: Line " << lineCount << " malformed. Only " << fields.size() << " field(s) found." << std::endl;
                continue;
            }
            if (fields[0].length() != 6)
            {
                std::cout << "WARN: Trade currencies on line " << lineCount << " malformed: '" << fields[0] << "'" << std::endl;
                continue;
            }
            int tradeAmount;
            if (!TryParseInt(fields[1], tradeAmount))
            {
                std::cout << "WARN: Trade amount on line " << lineCount << " not a valid integer: '" << fields[1] << "'" << std::endl;
            }
            double tradePrice;
            if (!TryParseDouble(fields[2], tradePrice))
            {
                std::cout << "WARN: Trade price on line " << lineCount << " not a valid decimal: '" << fields[2] << "'" << std::endl;
            }
            std::string scc = fields[0].substr(0, 3);
            std::string dcc = fields[0].substr(3, 3);
            // calculate values
            TradeRecord trade;
            trade.SourceCurrency = scc;
            trade.DestinationCurrency = dcc;
            trade.Lots = tradeAmount / LotSize;
            trade.Price = tradePrice;
            records.push_back(trade);
            lineCount++;
        }

        std::ostringstream xmlStream;
        xmlStream << "<TradeRecords>" << std::endl;
        for (const auto& record : records) {
        xmlStream << "\t<TradeRecord>" << std::endl;
        xmlStream << "\t\t<SourceCurrency>" << record.SourceCurrency << "</SourceCurrency>" << std::endl;
        xmlStream << "\t\t<DestinationCurrency>" << record.DestinationCurrency << "</DestinationCurrency>" << std::endl;
        xmlStream << "\t\t<Lots>" << record.Lots << "</Lots>" << std::endl;
        xmlStream << "\t\t<Price>" << record.Price << "</Price>" << std::endl;
        xmlStream << "\t</TradeRecord>" << std::endl;
       }
        xmlStream << "</TradeRecords>";
        std::string xmlData=xmlStream.str();
        std::ofstream outFile("output.xml"); // Output XML file
        outFile << xmlData;
        std::cout << "INFO: " << records.size() << " trades processed" << std::endl;
    }