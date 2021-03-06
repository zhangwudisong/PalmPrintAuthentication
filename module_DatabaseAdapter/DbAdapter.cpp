#include <module_DatabaseAdapter/DbAdapter.h>
#include <string>
#include <iostream>
#include <json/json11.hpp>
#include <module_DatabaseAdapter/JsonPoint.h>
#include "easylogging++.h"

using namespace sqlite;
using namespace json11;

DbAdapter::DbAdapter(){
	try{
		db = new database(dbName);
		
        // Creating table for the line features
		*db <<
			"create table if not exists palm_lines ("
			"   _id integer primary key autoincrement not null,"
			"   userId int,"
			"   featureData text"
			");";


         // Creating table for the texture features
		*db <<
            "create table if not exists palm_textures ("
			"   _id integer primary key autoincrement not null,"
			"   userId int,"
			"   featureData text"
			");";

	}
	catch (std::exception& e) {
        LOG(INFO) << e.what();
	}
}

void DbAdapter::insertTextureFeature(int userId, std::string jsonData){

	try{
        *db << "insert into palm_textures (userId, featureData) values (?,?);"
			<< userId
			<< jsonData;
	}
	catch (std::exception& e) {
        LOG(INFO) << e.what();
	}

}

void DbAdapter::insertLineFeature(int userId, std::string jsonData){

	try{
		*db  << "insert into palm_lines (userId, featureData) values (?,?);"
			 << userId
			 << jsonData;
	}
	catch (std::exception& e) {
        LOG(INFO) << e.what();
	}
	
}
vector < pair<int, vector<Point>>>  DbAdapter::getLineFeatures(int userId){
    vector <pair<int, vector<Point>>> records;
    try{
        *db << "select userId, featureData from palm_lines where userId = ?;"
            << userId
            >> [&](int userId, std::string featureData){
            std::string error;
            Json features = Json::parse(featureData, error);

            if (features.is_null()) {
                LOG(INFO) << "Line feature is null";
            }
            else{

                vector<Point> featurePoints;
                Json::array a = features.array_items();

                for (auto it = a.begin(); it < a.end(); ++it){
                    Json::array b = it->array_items();
                    Point p(b.at(0).int_value(), b.at(1).int_value());
                    featurePoints.push_back(p);
                }

                records.push_back(make_pair(userId, featurePoints));
            }
        };
    }
    catch (std::exception& e) {
        LOG(INFO) << e.what();
    }

    return records;

}

vector < pair<int, vector<Point>>>  DbAdapter::getLineFeatures(){
	vector <pair<int, vector<Point>>> records;
	try{
		*db << "select userId, featureData from palm_lines;"
			>> [&](int userId, std::string featureData){
			std::string error;
			Json features = Json::parse(featureData, error);

			if (features.is_null()) {
                LOG(INFO) << "Line feature is null";
			}
			else{

				vector<Point> featurePoints;
				Json::array a = features.array_items();

				for (auto it = a.begin(); it < a.end(); ++it){
					Json::array b = it->array_items();
					Point p(b.at(0).int_value(), b.at(1).int_value());
					featurePoints.push_back(p);
				}

				records.push_back(make_pair(userId, featurePoints));
			}
		};
	}
	catch (std::exception& e) {
        LOG(INFO) << e.what();
	}

	return records;

}

vector <pair<int, Json>> DbAdapter::getTextureFeatures(){
	vector <pair<int, Json>> records;

	try{
    *db << "select userId, featureData from palm_textures;"
		>> [&](int userId, std::string featureData){

			std::string error;
			Json feature = Json::parse(featureData, error);

			if (feature.is_null()) {
                LOG(INFO) << "Texture feature is null";
			}
			else{
				records.push_back(make_pair(userId, feature));
			}
		};
	}
	catch (std::exception& e) {
        LOG(INFO) << e.what();
	}

	return records;
}


vector <pair<int, Json>> DbAdapter::getTextureFeatures(int userId){
    vector <pair<int, Json>> records;

    try{
    *db << "select userId, featureData from palm_textures where userId = ?;"
        << userId
        >> [&](int userId, std::string featureData){

            std::string error;
            Json feature = Json::parse(featureData, error);

            if (feature.is_null()) {
                LOG(INFO) << "Texture feature is null";
            }
            else{
                records.push_back(make_pair(userId, feature));
            }
        };
    }
    catch (std::exception& e) {
        LOG(INFO) << e.what();
    }

    return records;
}


DbAdapter::~DbAdapter()
{
	delete db;
}
