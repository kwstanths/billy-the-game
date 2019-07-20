#include "OpenGLQuery.hpp"

game_engine::graphics::opengl::OpenGLQuery::OpenGLQuery() {

}

int game_engine::graphics::opengl::OpenGLQuery::Init() {

    glGenQueries(1, &query_);
    return 0;
}

int game_engine::graphics::opengl::OpenGLQuery::Destroy() {

    glDeleteQueries(1, &query_);

    return 0;
}

int game_engine::graphics::opengl::OpenGLQuery::Begin(int query_type){

    type = query_type;
    glBeginQuery(query_type, query_);

    return 0;
}

int game_engine::graphics::opengl::OpenGLQuery::End(){

    glEndQuery(type);

    return 0;
}

int game_engine::graphics::opengl::OpenGLQuery::GetResult() {

    int result;
    glGetQueryObjectiv(query_, GL_QUERY_RESULT, &result);

    return result;
}

bool game_engine::graphics::opengl::OpenGLQuery::IsResultReady() {
    
    int result;
    glGetQueryObjectiv(query_, GL_QUERY_RESULT_AVAILABLE, &result);

    return (result == GL_TRUE);
}
