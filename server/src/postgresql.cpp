#include "../inc/postgresql.h"
#include <format>
#include <string>
#include <vector>

std::string init = R"(
BEGIN;

CREATE TABLE IF NOT EXISTS public.documents
(
    document_id SERIAL,
    document_name character varying COLLATE pg_catalog."default" NOT NULL,
    last_edit_date date NOT NULL,
    CONSTRAINT documents_pkey PRIMARY KEY (document_id)
);

CREATE TABLE IF NOT EXISTS public.users
(
    user_id SERIAL,
    user_nickname character varying(64) COLLATE pg_catalog."default" NOT NULL,
    user_password character varying(64) COLLATE pg_catalog."default" NOT NULL,
    CONSTRAINT "user_id_PK" PRIMARY KEY (user_id),
    CONSTRAINT "username_UNIQUE" UNIQUE (user_nickname)
);

CREATE TABLE IF NOT EXISTS public.relations
(
    user_id integer NOT NULL,
    document_id integer NOT NULL,
    access_level_id integer NOT NULL,
    CONSTRAINT "user_id_document_id_PK" PRIMARY KEY (user_id, document_id)
);

CREATE TABLE IF NOT EXISTS public.access_level_classificator
(
    access_level_id integer NOT NULL,
    access_level character varying NOT NULL,
    CONSTRAINT "access_level_id_PK" PRIMARY KEY (access_level_id)
);

-- 1. Drop constraints if they already exist to prevent "constraint already exists" errors
ALTER TABLE IF EXISTS public.relations DROP CONSTRAINT IF EXISTS "user_id_FK";
ALTER TABLE IF EXISTS public.relations DROP CONSTRAINT IF EXISTS "document_id_FK";
ALTER TABLE IF EXISTS public.relations DROP CONSTRAINT IF EXISTS "access_level_id_FK";

-- 2. Recreate the constraints
ALTER TABLE IF EXISTS public.relations
    ADD CONSTRAINT "user_id_FK" FOREIGN KEY (user_id)
    REFERENCES public.users (user_id) MATCH SIMPLE
    ON UPDATE NO ACTION
    ON DELETE NO ACTION
    NOT VALID;

ALTER TABLE IF EXISTS public.relations
    ADD CONSTRAINT "document_id_FK" FOREIGN KEY (document_id)
    REFERENCES public.documents (document_id) MATCH SIMPLE
    ON UPDATE NO ACTION
    ON DELETE NO ACTION
    NOT VALID;

ALTER TABLE IF EXISTS public.relations
    ADD CONSTRAINT "access_level_id_FK" FOREIGN KEY (access_level_id)
    REFERENCES public.access_level_classificator (access_level_id) MATCH SIMPLE
    ON UPDATE NO ACTION
    ON DELETE NO ACTION
    NOT VALID;

-- 3. Insert initial classification values, ignoring them if they already exist
INSERT INTO access_level_classificator (access_level_id, access_level) VALUES 
	 (0, 'CREATOR'),
	 (1, 'EDIT'),
	 (2, 'READ')
ON CONFLICT (access_level_id) DO NOTHING;

END;    
)";

PostgresqlWorker::PostgresqlWorker():
connection("user=postgres password=admin host=localhost port=5432 dbname=practice target_session_attrs=read-write"){
    // pqxx::work w(connection);
    // pqxx::result r = w.exec(init);
    // w.commit();
}


ACTION_RESULT PostgresqlWorker::logUserPwd(std::string username, std::string pwd)
{
    pqxx::work w(connection);
    pqxx::result r = w.exec("SELECT user_password FROM users WHERE user_nickname = \'" +username+"\'");
    w.commit();

    if(r[0][0].is_null()){
        return USER_NOT_FOUND;
    }
    std::string true_pwd = r[0][0].c_str();
    if(true_pwd == pwd){
        return SUCCESS;
    }
    else{
        return WRONG_PASSWORD;
    }
}

ACTION_RESULT PostgresqlWorker::signUserPwd(std::string username, std::string password)
{
    pqxx::work w(connection);
    pqxx::params p (username, password);
    try{
        pqxx::result r = w.exec("INSERT INTO users (user_nickname, user_password) VALUES ($1, $2)", p);
        w.commit();    
    }
    catch(std::exception e){
        return USERNAME_TAKEN;
    }
    return SUCCESS;
}

ACTION_RESULT PostgresqlWorker::checkExistanceAndPermission(uint32_t file_id, uint32_t user_id)
{
    
    pqxx::work w_ex(connection);
    pqxx::params p (user_id, file_id);
    pqxx::result r = w_ex.exec("SELECT access_level_id FROM relations WHERE user_id = $1 AND document_id = $2", p);
    w_ex.commit();
    if(r.empty()) return FILE_NOT_FOUND;
    int level = r[0][0].as<int>();
    if(level == 0) return OWNER;
    if(level == 1) return PERMISSION_WRITE;
    if(level == 2) return PERMISSION_READ;
    else return PERMISSION_DENIED;

}

std::vector<std::pair<uint32_t, std::string>> PostgresqlWorker::getVectorOfFilesByUserID(uint32_t user_id)
{
    std::vector<std::pair<uint32_t, std::string>> res;
    pqxx::work w(connection);
    pqxx::result r = w.exec(
"SELECT relations.document_id, documents.document_name  FROM relations, documents \
WHERE relations.document_id = documents.document_id AND relations.user_id = " + std::to_string(user_id));
    w.commit();
    for(auto const& row: r){
        res.push_back({std::stoul(row[0].c_str()), row[1].c_str()});
    }
    return res;   
}
uint32_t PostgresqlWorker::getUserId(std::string username)
{
    pqxx::work w(connection);
    pqxx::result r = w.exec("SELECT user_id FROM users WHERE user_nickname = '" + username + "'");
    w.commit();
    return std::stoul(r[0][0].c_str());
}

std::string PostgresqlWorker::getUsername(uint32_t id)
{
    pqxx::work w(connection);
    pqxx::params p(id);
    pqxx::result r = w.exec("SELECT user_nickname FROM users WHERE user_id = $1", p);
    w.commit();
    return r[0][0].c_str();
}

std::vector<uint32_t> PostgresqlWorker::getOtherListenersOfFile(u_int32_t file_id)
{
    std::vector<uint32_t> res;
    pqxx::work w(connection);
    pqxx::params p(file_id);
    pqxx::result r = w.exec("SELECT user_id FROM relations WHERE document_id = $1", p);
    w.commit();
    for (const auto& row: r){
        res.push_back(row[0].as<uint32_t>());
    }
    return res;
}

std::vector<std::pair<uint32_t, std::string>> PostgresqlWorker::getAllUsersOfFile(uint32_t file_id)
{
    std::vector<std::pair<uint32_t, std::string>>  res;
    pqxx::work w(connection);
    pqxx::params p(file_id);
    pqxx::result r = w.exec("SELECT user_id, access_level_id FROM relations WHERE document_id = $1", p);
    w.commit();
    for (const auto& row: r){
        res.push_back({row[0].as<uint32_t>(), row[1].c_str()});
    }
    return res;
}

uint32_t PostgresqlWorker::getFileCreatorId(uint32_t file_id)
{
    pqxx::work w(connection);
    pqxx::params p(file_id);
    pqxx::result r = w.exec("SELECT user_id FROM relations WHERE document_id = $1 AND access_level_id = 0", p);
    w.commit();

    return r[0][0].as<uint32_t>();
}
ACTION_RESULT PostgresqlWorker::changeAccessLevelForUser(uint32_t file_id, std::string username, int shared_level)
{
    pqxx::work w1(connection);
    pqxx::params p1 (username);
    pqxx::result r1 = w1.exec(R"(
        SELECT EXISTS (
            SELECT 1 
            FROM users 
            WHERE user_nickname = $1
    );)", p1);
    w1.commit();
    if(!r1[0][0].as<bool>()) {
        return USER_NOT_FOUND;
    }
    uint32_t user_id = getUserId(username);
    ACTION_RESULT level = checkExistanceAndPermission(file_id, user_id);

    pqxx::work w(connection);
    pqxx::params p (shared_level, user_id, file_id), p2(user_id, file_id);
    if(shared_level == 3){
        pqxx::result r = w.exec("DELETE FROM relations WHERE user_id = $1 AND document_id = $2", p2);
        w.commit();
        return SUCCESS;
    }
    else if(level == FILE_NOT_FOUND){
        pqxx::result r = w.exec("INSERT INTO relations (user_id, document_id, access_level_id) VALUES \
             ($2, $3, $1) ON CONFLICT (user_id, document_id) DO UPDATE SET access_level_id = EXCLUDED.access_level_id", p);
        w.commit();
        return SUCCESS;
    }
    else if(level == OWNER){
        return CANT_CHANGE_OWNER_LEVEL;
    }
    else{
        pqxx::result r = w.exec("UPDATE relations SET access_level_id = $1 WHERE user_id = $2 AND document_id = $3", p);
        w.commit();
        return SUCCESS;
    }
}

ACTION_RESULT PostgresqlWorker::deleteFile(uint32_t file_id, uint32_t user_id, ACTION_RESULT permission)
{   
    pqxx::work w(connection);
    pqxx::params p(file_id, user_id), p1(file_id);
    switch (permission)
    {
    case PERMISSION_WRITE:
    case PERMISSION_READ:
        w.exec("DELETE FROM relations WHERE document_id = $ 1 AND user_id = $2", p);
        w.commit();
        return SUCCESS;
    case OWNER:
        w.exec("DELETE FROM relations WHERE document_id = $1;", p1);
        w.exec("DELETE FROM documents WHERE document_id = $1;", p1);
        w.commit();
        return SUCCESS;
    default:
        break;
    }
    return FAILURE;
}

ACTION_RESULT PostgresqlWorker::renameFile(uint32_t file_id, uint32_t user_id, std::string newName)
{
    ACTION_RESULT level = checkExistanceAndPermission(file_id, user_id);
    if(level == PERMISSION_WRITE || level == OWNER){
        pqxx::work w(connection);
        pqxx::params p (newName, file_id);
        pqxx::result r = w.exec("UPDATE documents SET document_name = $1 WHERE document_id = $2", p);
        w.commit();
        return SUCCESS;
    }
    else {
        return FAILURE;
    }
}


uint32_t PostgresqlWorker::registerFile(std::string newName, uint32_t user_id)
{
    uint32_t file_id;
    pqxx::work w(connection);
    try{
        pqxx::params p (newName);
        pqxx::result r = w.exec("insert into documents (document_name, last_edit_date) VALUES \
            ($1, CURRENT_DATE) returning document_id", p);
        
        file_id =  r[0][0].as<uint32_t>();
        p = pqxx::params(user_id, file_id);

        w.exec("insert into relations (user_id, document_id, access_level_id) VALUES"
            "($1, $2, 0)", p);  
        w.commit();
    }
    catch(std::exception e){
        return 0;
    }
    return file_id;
}
