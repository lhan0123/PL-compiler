# include <iostream>
# include <string>
# include <string.h>
# include <vector>
# include <cstdlib>
# include <stdio.h>
# include <iomanip>

using namespace std;

bool gisEOF = false ;   

char gcurchar = ' ' ;    
char gnextchar = ' ' ;  

int gLine = 1; 
int gColumn = 1 ; 
int gcurLine = 1 ; 
int gcurColumn = 1 ; 
int gtokenLine = 1 ; 
int gtokenColumn = 1 ; 



enum TOKENTYPE
{
  IDENTIFIER, 
  CONSTANT_INT, CONSTANT_FLOAT, CONSTANT_CHAR, CONSTANT_STRING, CONSTANT_BOOL,
  INT, FLOAT, CHAR, BOOL, STRING, VOID,
  IF, ELSE, WHILE, DO, RETURN,
  LP, RP, LBK, RBK, LBC, RBC,
  PLUS, MINUS, MUL, DIV, MOD, POWER,
  GT, ST, GE, LE, EQ, NEQ,
  LAND, LOR, NOT, AND, OR,
  PE, ME, TE, DE, RE, PP, MM, RS, LS,
  SEMICOLON, COMMA, QMARK, COLON,
  ASSIGN,
  ARRAY, VARIABLE, CONSTANT, FUNCTION, 
  TOKEN_IS_EOF,
  UNRECOGNIZE_TOKEN
};

enum USERINPUTTYPE {
  DEFINESYMBOL, DEFINEFUNCTION, STATEMENT, LOCALSTATEMENT
};


struct TOKEN_CHAR{
  char tokenCHAR ;
  int  line  ;
  int  column  ;
};

struct TOKEN{
  string tokenID ;    
  TOKENTYPE tokentype ;  
  int tokenLine  ;       
  int tokenColumn  ;     
  
};

TOKEN gToken ;     
TOKEN gPeekToken ; 



struct IntOrFloatSymbolInfo{
  string name ;
  double value ;
};

vector<IntOrFloatSymbolInfo> gIntTable ;  
vector<IntOrFloatSymbolInfo> gFloatTable ; 

struct CharSymbolInfo{
  string name ;
  char value ;
};

vector<CharSymbolInfo> gCharTable ; 

struct BoolSymbolInfo{
  string name ;
  bool value ;
};

vector<BoolSymbolInfo> gBoolTable ; 

struct StringSymbolInfo{
  string name ;
  string value ;
};

vector<StringSymbolInfo> gStringTable ; 

struct ArrayInfo{
  
  int table ;   
  int entry ;
  
  
  int size ;
};

vector<ArrayInfo> gArrayInfoTable ; 

struct ArgsDefineInfo{
  string name ;
  TOKENTYPE argType ; 
  TOKENTYPE argReturnType ; 
  bool isReturn ;           
} ;

struct ArgsComputeInfo{
  string name ;       
  TOKENTYPE argType ; 
  TOKENTYPE argReturnType ; 
  int table ;    
  int entry ;   
  
};

struct IndentifierSymbolInfo{
  string name ;          
  TOKENTYPE symbolType ; 
  TOKENTYPE symbolReturnType ; 
  int symbolStartLine ;        
  int symbolStartColumn ;      
  int table ;            
  int entry ;           
};


struct FunctionInfo{
  string name ;                      
  TOKENTYPE funcSymbolReturnType ;  
  int funcStartLine ;               
  int funcStartColumn ;            
  int funcEndLine ;                
  int funcEndColumn ;              
  vector<ArgsDefineInfo> argDefineList ; 
  vector<IndentifierSymbolInfo> localSymbolList ; 
};


vector<IndentifierSymbolInfo> gGlobalSymbolList ;      
vector<FunctionInfo> gGlobalFuncSymbolList ;      
vector<FunctionInfo> gCallStack ;

vector<TOKEN> gAllTokenStringList ; 

class Exception {
public:
  string message;  
   
  Exception() {  }

  Exception( string wrongToken ) {
    message = wrongToken;
  } 

}; 


class Exception1 : public Exception {
public:
  Exception1( string wrongToken ) {
    message = wrongToken ;
  } 
}; 


class Exception2 : public Exception {
public:
  Exception2( string wrongToken ) {
    message = wrongToken ;
  } 
}; 


class Exception3 : public Exception { 
public:
  Exception3( string wrongToken ) {
    message = wrongToken ;
  } 
}; 


class Exception4 : public Exception {
public:
  Exception4( string wrongToken ) {
    message = wrongToken ;
  } 
}; 


class SYNTAX{
public:
  bool mIsCompute ; 
  FunctionInfo mNewDefineFunc ; 
  vector<IndentifierSymbolInfo> mNewDefineSymbolList ; 
  vector<IndentifierSymbolInfo> mStmtNewDefineSymbolList ; 
  USERINPUTTYPE mUserInputType ; 
  
  
  void FunctionInit( FunctionInfo &newDefineFunc ) {
    newDefineFunc.name.clear() ;
    newDefineFunc.funcSymbolReturnType = UNRECOGNIZE_TOKEN ;
    newDefineFunc.funcStartColumn = -1 ;
    newDefineFunc.funcStartColumn = -1 ;
    newDefineFunc.argDefineList.clear() ;
    newDefineFunc.localSymbolList.clear() ;
  } 
  
  void IndentifierInit( IndentifierSymbolInfo &newDefineSymbol ) {
    newDefineSymbol.name.empty() ;
    newDefineSymbol.symbolReturnType = UNRECOGNIZE_TOKEN  ;
    newDefineSymbol.symbolType = UNRECOGNIZE_TOKEN ;
    newDefineSymbol.symbolStartLine = -1 ;
    newDefineSymbol.symbolStartColumn = -1 ;
    newDefineSymbol.entry = -1 ;
    newDefineSymbol.table = -1 ;
  } 
  
  
  void TokenInit( TOKEN &token ) {
    token.tokenID = "" ;
    token.tokenLine = 0 ;
    token.tokenColumn = 0 ;
    token.tokentype = UNRECOGNIZE_TOKEN ;
    
  } 
  
  
  bool Get_next_ch( char &ch ) {
  
    if ( scanf( "%c", &ch ) == EOF ) {
      return false ;
    } 
  
    else {
       
      gcurLine = gLine ;
      gcurColumn = gColumn ;
      
      if ( ch == '\n' ) {
        gLine = gLine + 1 ; 
        gColumn = 1 ;      
        
      } 

      
      else {
        gColumn = gColumn + 1 ; 
      } 
    } 
  
    return true ; 
  } 


  
  
  
  
  bool Get_next_non_white_space_ch( char &curchar, char &nextchar ) {
    bool isBreak = false ;
  
    curchar = nextchar ;
  
    
    while ( ( curchar == '\n' || curchar == '\t' || curchar == ' ' || curchar == '/' ) && !isBreak ) {
      if ( curchar == '/' ) { 
        if ( Get_next_ch( nextchar ) ) { 
          if ( nextchar == '/' ) { 
          
            nextchar = '\0' ;
            while ( curchar != '\n' ) { 
              
              if ( !Get_next_ch( curchar ) ) { 
                gisEOF = true ; 
                return false ; 
              } 
  
            } 
          } 

          else isBreak = true ; 
        } 

        else { 
          gisEOF = true ;  
          isBreak = true ; 
        } 
      } 

      else { 
        
        if ( !Get_next_ch( curchar ) ) {  
          gcurLine = gLine ;
          gcurColumn = gColumn ;
          gisEOF = true ;
          return false ;
        } 
      } 
    } 

    gtokenLine = gcurLine ;
    if ( curchar == '/' ) {
      gtokenColumn = gcurColumn-1 ;
    } 
    else
      gtokenColumn = gcurColumn ;

    return true ; 
  } 

  
  
  void Get_ID_token( TOKEN &t ) { 
    
    string token = "" ;
    bool isBreak = false ;
    int startLine = gtokenLine ;
    int startColumn = gtokenColumn ;

    
    
    
    
    
  
    
    token = token + gcurchar ;
  
    
    if ( Get_next_ch( gnextchar ) ) {
    
      
      while ( ( ( gnextchar >= 'a' && gnextchar <= 'z' ) || ( gnextchar >= 'A' && gnextchar <= 'Z' ) 
                || ( gnextchar >= '0' && gnextchar <= '9' ) || gnextchar == '_' ) && !isBreak ) {
                 
        
        gcurchar = gnextchar ; 
      
        
        
        
        
        
      
        
        token = token + gcurchar ;
      
        
        if ( !Get_next_ch( gnextchar ) ) { 
          gisEOF = true ; 
          isBreak = true ; 
        } 
      } 
    } 
  
    
    else { 
      gisEOF = true ;
    } 
  
    
    t.tokenID = token ;
    
    
    
    t.tokenLine = startLine ;
    t.tokenColumn = startColumn ;
  } 

  
  
  void Get_NUM_token( TOKEN &t ) {
    
    string token = "" ;
    bool isBreak = false ;
    int startLine = gtokenLine ;
    int startColumn = gtokenColumn ;
    
    
    
    
    
    
    
  
    
    token = token + gcurchar ;
  
    
    if ( Get_next_ch( gnextchar ) ) { 
  
      
      while ( ( gnextchar >= '0' && gnextchar <= '9' ) && !isBreak ) {
        
        gcurchar = gnextchar ; 
      
        
        
        
        
        
      
        
        token = token + gcurchar ;
      
        
        if ( !Get_next_ch( gnextchar ) ) { 
          gisEOF = true ; 
          isBreak = true ; 
        } 
      } 

      isBreak = false ;  
    
      
     
      
      if ( !gisEOF && gnextchar == '.' ) {
        
        gcurchar = gnextchar ;  
      
        
        
        
        
        
      
        
        token = token + gcurchar ;
      
        
        if ( Get_next_ch( gnextchar ) ) {

          
          while ( ( gnextchar >= '0' && gnextchar <= '9' ) && !isBreak ) {
          
            
            gcurchar = gnextchar ;  
          
            
            
            
            
            
          
            
            token = token + gcurchar ;
          
            
            if ( !Get_next_ch( gnextchar ) ) { 
              gisEOF = true ; 
              isBreak = true ; 
            } 
          } 
        } 
      
        
        else {  
          gisEOF = true ;
        } 
      } 
    } 
  
    
    else { 
      gisEOF = true ;
    } 
  
    
    t.tokenID = token ;
    
    
    t.tokenLine = startLine ;
    t.tokenColumn = startColumn ;

  } 


  
  void Get_Special_ch_token( TOKEN &t ) {
    TOKEN_CHAR tc ;
    string token = "" ;
    bool isBreak = false ;
    int slash_count = 0 ; 
    
    int startLine = gtokenLine ;
    int startColumn = gtokenColumn ;
  
    
    if ( gcurchar == '\'' ) {
    
      
      
      
      
      
    
      
      token = token + gcurchar ;
    
      
      if ( Get_next_ch( gnextchar ) ) {
      
        
        while ( !isBreak ) {
        
          
          gcurchar = gnextchar ;
        
          
          
          
          
          
        
          
          token = token + gcurchar ;
        
          
          if ( gcurchar == '\'' ) {
            isBreak = true ; 
          } 
          
          
          if ( !Get_next_ch( gnextchar ) ) {
            gisEOF = true ; 
            isBreak = true ; 
          } 
        } 
      } 

      else { 
        gisEOF = true ;
      } 
    } 
  
    
    else if ( gcurchar == '"' ) {
      
      
      
      
      
    
      
      token = token + gcurchar ;
    
      
      if ( Get_next_ch( gnextchar ) ) {
        
        
        while ( !isBreak ) {
          
          gcurchar = gnextchar ;  
        
          
          
          
          
          
        
          
          token = token + gcurchar ;
        
          
          if ( gcurchar == '"' ) {
            isBreak = true ; 
          } 
        
          
          if ( !Get_next_ch( gnextchar ) ) {   
            gisEOF = true ; 
            isBreak = true ; 
          } 
        } 
      } 

      else { 
        gisEOF = true ;
      } 
    } 
  
    
    else if ( gcurchar == '.' ) {
    
      
      
      
      
      
    
      
      token = token + gcurchar ;
    
      
      if ( Get_next_ch( gnextchar ) ) {
      
        
        while ( ( gnextchar >= '0' && gnextchar <= '9' ) && !isBreak ) {
          
          gcurchar = gnextchar ;  
        
          
          
          
          
          
        
          
          token = token + gcurchar ;
        
          
          if ( !Get_next_ch( gnextchar ) ) {   
            gisEOF = true ; 
            isBreak = true ; 
          } 
        } 
      } 
   
      else { 
        gisEOF = true ;
      } 
    } 
  
    
    else if ( gcurchar == '+' ) {
      
      
      
      
      
    
      
      token = token + gcurchar ;
    
      
      if ( Get_next_ch( gnextchar ) ) {
        
        if ( gnextchar == '=' || gnextchar == '+' ) {
          
          gcurchar = gnextchar ;   
        
          
          
          
          
          
        
          
          token = token + gcurchar ;   
          if ( !Get_next_ch( gnextchar ) ) { 
            gisEOF = true ; 
          } 
        } 
      
        
        
      } 

      else { 
        gisEOF = true ;
      } 
    } 
  
    
    else if ( gcurchar == '-' ) {
      
      
      
      
      
    
      
      token = token + gcurchar ;
    
      
      if ( Get_next_ch( gnextchar ) ) {
        
        if ( gnextchar == '=' || gnextchar == '-' ) {
          
          gcurchar = gnextchar ;   
        
          
          
          
          
          
        
          
          token = token + gcurchar ;
        
          
          if ( !Get_next_ch( gnextchar ) ) { 
            gisEOF = true ; 
          } 
        } 

        
        
      } 

      else { 
        gisEOF = true ;
      } 
    } 
  
    
    else if ( gcurchar == '*' ) {
      
      
      
      
      
    
      
      token = token + gcurchar ;
    
      
      if ( Get_next_ch( gnextchar ) ) {
        
        if ( gnextchar == '=' ) {
          
          gcurchar = gnextchar ;   
        
          
          
          
          
          
        
          
          token = token + gcurchar ;
        
          
          if ( !Get_next_ch( gnextchar ) ) { 
            gisEOF = true ; 
          } 
        } 

        
        
      } 

      else { 
        gisEOF = true ;
      } 
    } 
  
    
    else if ( gcurchar == '/' ) {
      
      
      
      
      
    
      
      token = token + gcurchar ;
    
      
      
      if ( gnextchar == '=' ) {
        
        gcurchar = gnextchar ;
      
        
        
        
        
        
      
        
        token = token + gcurchar ;
      
        
        if ( !Get_next_ch( gnextchar ) ) { 
          gisEOF = true ; 
        } 
      } 
    
      
      
    } 
  
    
    else if ( gcurchar == '%' ) {
      
      
      
      
      
    
      
      token = token + gcurchar ;
    
      
      if ( Get_next_ch( gnextchar ) ) { 
        
        if ( gnextchar == '=' ) {
          
          gcurchar = gnextchar ; 
        
          
          
          
          
          
        
          
          token = token + gcurchar ;
        
          
          if ( !Get_next_ch( gnextchar ) ) { 
            gisEOF = true ; 
          } 
        } 

        
        
      } 

      else { 
        gisEOF = true ;
      } 
    } 
  
    
    else if ( gcurchar == '>' ) {
      
      
      
      
      
    
      
      token = token + gcurchar ;
    
      
      if ( Get_next_ch( gnextchar ) ) {
        
        if ( gnextchar == '=' || gnextchar == '>' ) {
          
          gcurchar = gnextchar ; 
        
          
          
          
          
          
        
          
          token = token + gcurchar ;
        
          
          if ( !Get_next_ch( gnextchar ) ) { 
            gisEOF = true ; 
          } 
        } 

        
        
      } 

      else { 
        gisEOF = true ;
      } 
    } 
  
    
    else if ( gcurchar == '<' ) {
      
      
      
      
      
    
      
      token = token + gcurchar ;
    
      
      if ( Get_next_ch( gnextchar ) ) {
        
        if ( gnextchar == '<' || gnextchar == '=' ) {
          
          gcurchar = gnextchar ; 
        
          
          
          
          
          
        
          
          token = token + gcurchar ;
        
          
          if ( !Get_next_ch( gnextchar ) ) { 
            gisEOF = true ; 
          } 
        } 

        
        
      } 

      else { 
        gisEOF = true ;
      } 
    } 
  
    
    else if ( gcurchar == '=' ) {
      
      
      
      
      
    
      
      token = token + gcurchar ;
    
      
      if ( Get_next_ch( gnextchar ) ) {
        
        if ( gnextchar == '=' ) {
          
          gcurchar = gnextchar ; 
        
          
          
          
          
          
        
          
          token = token + gcurchar ;
        
          
          if ( !Get_next_ch( gnextchar ) ) { 
            gisEOF = true ; 
          } 
        } 

        
        
      } 

      else { 
        gisEOF = true ;
      } 
    } 
  
    
    else if ( gcurchar == '!' ) {
      
      
      
      
      
    
      
      token = token + gcurchar ;
    
      
      if ( Get_next_ch( gnextchar ) ) {
        
        if ( gnextchar == '=' ) {
          
          gcurchar = gnextchar ;
        
          
          
          
          
          
        
          
          token = token + gcurchar ;
        
          
          if ( !Get_next_ch( gnextchar ) ) { 
            gisEOF = true ; 
          } 
        } 

        
        
      } 

      else { 
        gisEOF = true ;
      } 
    } 
  
    
    else if ( gcurchar == '&' ) {
      
      
      
      
      
    
      
      token = token + gcurchar ;
    
      
      if ( Get_next_ch( gnextchar ) ) {
        
        if ( gnextchar == '&' ) {
          
          gcurchar = gnextchar ;
        
          
          
          
          
          
        
          
          token = token + gcurchar ;
        
          
          if ( !Get_next_ch( gnextchar ) ) { 
            gisEOF = true ; 
          } 
        } 

        
        
      } 

      else { 
        gisEOF = true ;
      } 
    } 
  
    
    else if ( gcurchar == '|' ) {
      
      
      
      
      
    
      
      token = token + gcurchar ;
    
      
      if ( Get_next_ch( gnextchar ) ) {
        
        if ( gnextchar == '|' ) {
          
          gcurchar = gnextchar ; 
        
          
          
          
          
          
        
          
          token = token + gcurchar ;
        
          
          if ( !Get_next_ch( gnextchar ) ) { 
            gisEOF = true ; 
          } 
        } 

        
        
      } 

      else { 
        gisEOF = true ;
      } 
    } 

    
    else {
      
      
      
      
      

      
      token = token + gcurchar ;
    
      
      if ( !Get_next_ch( gnextchar ) )  
        gisEOF = true ; 
    } 
  
    
    t.tokenID = token;
    
    
    
    t.tokenLine = startLine ;
    t.tokenColumn = startColumn ;
  } 


  
  
  
  bool Gettoken( TOKEN &t ) {
    string token = "" ;  
  
    if ( !gisEOF ) {
      if ( Get_next_non_white_space_ch( gcurchar, gnextchar ) ) {
        
        if ( ( gcurchar >= 'a' && gcurchar <= 'z' ) || ( gcurchar >= 'A' && gcurchar <= 'Z' ) ) {
          Get_ID_token( t ) ; 
        } 
  
        
        else if ( gcurchar >= '0' && gcurchar <= '9' ) {
          Get_NUM_token( t ) ; 
        } 
  
        
        else {
          Get_Special_ch_token( t ) ; 
        } 
      
        return true ; 
      } 
    } 
  
    return false ; 
  } 

  
  bool IsInteger( string tokenID ) {
    for ( int i = 0 ; i < tokenID.size() ; i++ ) {
      if ( tokenID.at( i ) < '0' || tokenID.at( i ) > '9' ) {
        return false ;
      } 
    } 

    return true ;
  } 

  
  bool HasOneDot( string tokenID ) {
    int dotCount = 0 ;

    for ( int i = 0; i < tokenID.size() ; i++ ) {
      if ( tokenID.at( i ) == '.' )
        dotCount++ ;
    } 

    if ( dotCount == 1 ) return true ;

    return false ;
  } 

  
  bool IsFloat( string tokenID ) {
    if ( HasOneDot( tokenID ) && tokenID.size() != 1 ) {  
      for ( int i = 0 ; i < tokenID.size() ; i++ ) {
        if ( ! ( ( tokenID.at( i ) >= '0' && tokenID.at( i ) <= '9' ) || tokenID.at( i ) == '.' ) ) {  
          return false ;
        } 
      } 

      return true ;
    } 

    return false ;
  } 
  
  
  bool IsIdentifier( string tokenID ) {
    if ( ( tokenID.at( 0 ) >= 'a' && tokenID.at( 0 ) <= 'z' ) 
         || ( tokenID.at( 0 ) >= 'A' && tokenID.at( 0 ) <= 'Z' ) ) {
      for ( int i = 1 ; i < tokenID.size() ; i++ ) {
        if ( ! ( ( tokenID.at( i ) >= 'a' && tokenID.at( i ) <= 'z' )
                 || ( tokenID.at( i ) >= 'A' && tokenID.at( i ) <= 'Z' )
                 || ( tokenID.at( i ) >= '0' && tokenID.at( i ) <= '9' )
                 || tokenID.at( i ) == '_' ) ) {

          return false ;
        } 
      } 

      return true ;
    } 

    return false ;
  } 
  
  void GetTokenType( string tokenID, TOKENTYPE &tokentype ) {
    if ( tokenID.compare( "int" ) == 0 ) tokentype = INT ;
    else if ( tokenID.compare( "float" ) == 0 ) tokentype = FLOAT ;
    else if ( tokenID.compare( "char" ) == 0 ) tokentype = CHAR ;
    else if ( tokenID.compare( "bool" ) == 0 ) tokentype = BOOL ;
    else if ( tokenID.compare( "string" ) == 0 ) tokentype = STRING ;
    else if ( tokenID.compare( "void" ) == 0 ) tokentype = VOID ;
    else if ( tokenID.compare( "if" ) == 0 ) tokentype = IF ;
    else if ( tokenID.compare( "else" ) == 0 ) tokentype = ELSE ;
    else if ( tokenID.compare( "while" ) == 0 ) tokentype = WHILE ;
    else if ( tokenID.compare( "do" ) == 0 ) tokentype = DO ;
    else if ( tokenID.compare( "return" ) == 0 ) tokentype = RETURN ;
    else if ( tokenID.compare( "true" ) == 0 ) tokentype = CONSTANT_BOOL ;
    else if ( tokenID.compare( "false" ) == 0 ) tokentype = CONSTANT_BOOL ;
    else if ( IsInteger( tokenID ) ) tokentype = CONSTANT_INT ;
    else if ( IsFloat( tokenID ) ) tokentype = CONSTANT_FLOAT ;
    else if ( tokenID.at( 0 ) == '\'' && tokenID.at( tokenID.size()-1 ) == '\'' ) 
      tokentype = CONSTANT_CHAR ;
    else if ( tokenID.at( 0 ) == '"' &&  tokenID.at( tokenID.size()-1 ) == '"' )  
      tokentype = CONSTANT_STRING ;
    else if ( IsIdentifier( tokenID ) ) tokentype = IDENTIFIER ;
    else if ( tokenID.compare( "(" ) == 0 ) tokentype = LP ;
    else if ( tokenID.compare( ")" ) == 0 ) tokentype = RP ;
    else if ( tokenID.compare( "[" ) == 0 ) tokentype = LBK ;
    else if ( tokenID.compare( "]" ) == 0 ) tokentype = RBK ;
    else if ( tokenID.compare( "{" ) == 0 ) tokentype = LBC ;
    else if ( tokenID.compare( "}" ) == 0 ) tokentype = RBC ;
    else if ( tokenID.compare( "+" ) == 0 ) tokentype = PLUS ;
    else if ( tokenID.compare( "-" ) == 0 ) tokentype = MINUS ;
    else if ( tokenID.compare( "*" ) == 0 ) tokentype = MUL ;
    else if ( tokenID.compare( "/" ) == 0 ) tokentype = DIV ;
    else if ( tokenID.compare( "%" ) == 0 ) tokentype = MOD ;
    else if ( tokenID.compare( "^" ) == 0 ) tokentype = POWER ;
    else if ( tokenID.compare( ">" ) == 0 ) tokentype = GT ;
    else if ( tokenID.compare( "<" ) == 0 ) tokentype = ST ;
    else if ( tokenID.compare( ">=" ) == 0 ) tokentype = GE ;
    else if ( tokenID.compare( "<=" ) == 0 ) tokentype = LE ;
    else if ( tokenID.compare( "==" ) == 0 ) tokentype = EQ ;
    else if ( tokenID.compare( "!=" ) == 0 ) tokentype = NEQ ;
    else if ( tokenID.compare( "&" ) == 0 ) tokentype = LAND ;
    else if ( tokenID.compare( "|" ) == 0 ) tokentype = LOR ;
    else if ( tokenID.compare( "=" ) == 0 ) tokentype = ASSIGN ;
    else if ( tokenID.compare( "!" ) == 0 ) tokentype = NOT ;
    else if ( tokenID.compare( "&&" ) == 0 ) tokentype = AND ;
    else if ( tokenID.compare( "||" ) == 0 ) tokentype = OR ;
    else if ( tokenID.compare( "+=" ) == 0 ) tokentype = PE ;
    else if ( tokenID.compare( "-=" ) == 0 ) tokentype = ME ;
    else if ( tokenID.compare( "*=" ) == 0 ) tokentype = TE ;
    else if ( tokenID.compare( "/=" ) == 0 ) tokentype = DE ;
    else if ( tokenID.compare( "%=" ) == 0 ) tokentype = RE ;
    else if ( tokenID.compare( "++" ) == 0 ) tokentype = PP ;
    else if ( tokenID.compare( "--" ) == 0 ) tokentype = MM ;
    else if ( tokenID.compare( ">>" ) == 0 ) tokentype = RS ;
    else if ( tokenID.compare( "<<" ) == 0 ) tokentype = LS ;
    else if ( tokenID.compare( ";" ) == 0 ) tokentype = SEMICOLON ;
    else if ( tokenID.compare( "," ) == 0 ) tokentype = COMMA ;
    else if ( tokenID.compare( "?" ) == 0 ) tokentype = QMARK ;
    else if ( tokenID.compare( ":" ) == 0 ) tokentype = COLON ;
    else tokentype = UNRECOGNIZE_TOKEN ; 
  } 
  
  
  
  void GetToken( TOKENTYPE &tokentype ) {
    
    
    
    if ( gPeekToken.tokenID.empty() ) {
      TokenInit( gToken ) ;
      GetTokenAndType( gToken ) ;
    } 
    
    
    else {
      TokenInit( gToken ) ;
      gToken = gPeekToken ;
      TokenInit( gPeekToken ) ;
    } 
    
    
    tokentype = gToken.tokentype ;
  } 
  
  void PassTwoGetToken( int nextTokenIndex, TOKENTYPE &tokentype ) {
    
    TokenInit( gToken ) ;
    
    if ( nextTokenIndex < gAllTokenStringList.size() ) {
      gToken = gAllTokenStringList[nextTokenIndex] ;
    } 
    
    else {
      gToken.tokentype = TOKEN_IS_EOF ;
    } 
    
    tokentype = gToken.tokentype ;
  } 
  
  void PassTwoPeekToken( int nextTokenIndex, TOKENTYPE &tokentype ) {
    
    TokenInit( gPeekToken ) ;
    
    if ( nextTokenIndex < gAllTokenStringList.size() ) {
      gPeekToken = gAllTokenStringList[nextTokenIndex] ;
    } 
    
    else {
      gPeekToken.tokentype = TOKEN_IS_EOF ;
    } 
    
    tokentype = gPeekToken.tokentype ;
  } 
  
  
  
  void PeekToken( TOKENTYPE &tokentype ) {
    
    
    
    if ( gPeekToken.tokenID.empty() ) {
      PeekTokenAndType( gPeekToken ) ;
    } 
    
    
    tokentype = gPeekToken.tokentype ;
  } 
  
  
  void GetTokenAndType( TOKEN &t ) {
    if ( Gettoken( t ) ) {
      GetTokenType( t.tokenID, t.tokentype )  ; 
    } 
    
    else {
      t.tokentype = TOKEN_IS_EOF ; 
    } 
    
  } 
  
  void PeekTokenAndType( TOKEN &t ) {
    
    if ( Gettoken( t ) ) { 
      GetTokenType( t.tokenID, t.tokentype )  ; 
    } 
    
    else {
      t.tokentype = TOKEN_IS_EOF ; 
    } 
  } 
  
  
  int FindTokenInAllTokenStringList( int tokenLine, int tokenColumn ) {
    for ( int i = 0 ; i < gAllTokenStringList.size() ; i ++ ) {
      if ( gAllTokenStringList[i].tokenLine == tokenLine 
           && gAllTokenStringList[i].tokenColumn == tokenColumn )
        return i ;
    } 
    
    return -1 ;
  } 
  
  
  int FindTokenInSymbolList( string token, vector<IndentifierSymbolInfo> symbolList ) {
    string acutalToken = "" ;
    
    if ( token.at( 0 ) == '"' )
      for ( int i = 1 ; i < token.size()-1 ; i++ ) 
        acutalToken = acutalToken + token.at( i ) ;
    else
      acutalToken = token ;
      
    for ( int i = 0 ; i < symbolList.size() ; i++ ) {
      if ( acutalToken.compare( symbolList[i].name ) == 0 ) return i ;
    } 
    
    return -1 ;
  } 
  
  
  int FindTokenInFunctionList( string token ) {
    string acutalToken = "" ;
    if ( token.at( 0 ) == '"' )
      for ( int i = 1 ; i < token.size()-1 ; i++ ) 
        acutalToken = acutalToken + token.at( i ) ;
    else
      acutalToken = token ;
    
    for ( int i = 0 ; i < gGlobalFuncSymbolList.size() ; i++ ) {
      if ( acutalToken.compare( gGlobalFuncSymbolList[i].name ) == 0 ) return i ;
    } 
    
    return -1 ;
  } 
  
  
  
  void InsertNewDenfineFunctionToGlobalFuncList( FunctionInfo mNewDefineFunc ) {
    int index = 0 ;
    
    index = FindTokenInFunctionList( mNewDefineFunc.name ) ;
    
    
    
    if ( index != -1 ) {
      
      gGlobalFuncSymbolList.erase( gGlobalFuncSymbolList.begin() + index ) ;
      
      gGlobalFuncSymbolList.push_back( mNewDefineFunc ) ;
      
      cout << "New definition of " <<  mNewDefineFunc.name << "() entered ..." << endl ;
    } 
    
    else {
      
      index = FindTokenInSymbolList( mNewDefineFunc.name, gGlobalSymbolList ) ;
      
      
      if ( index != -1 ) {
        
        gGlobalSymbolList.erase( gGlobalSymbolList.begin() + index ) ;
        
        gGlobalFuncSymbolList.push_back( mNewDefineFunc ) ;
        
        cout << "New definition of " <<  mNewDefineFunc.name << "() entered ..." << endl ;
      } 
      
      
      else {
        
        gGlobalFuncSymbolList.push_back( mNewDefineFunc ) ;
        
        cout << "Definition of " <<  mNewDefineFunc.name << "() entered ..." << endl ;
      } 
    } 
  } 
  
  
  
  void InsertNewDenfineSymbolToGlobalSymbolList( IndentifierSymbolInfo symbolInfo ) {
    int index = 0 ;
    
    index = FindTokenInSymbolList( symbolInfo.name, gGlobalSymbolList ) ;
    
    
    
    if ( index != -1 ) {
      
      gGlobalSymbolList.erase( gGlobalSymbolList.begin() + index ) ;
      
      gGlobalSymbolList.push_back( symbolInfo ) ;
      
      cout << "New definition of " <<  symbolInfo.name << " entered ..." << endl ;
    } 
    
    else {
      
      index = FindTokenInFunctionList( symbolInfo.name ) ;
      
      
      if ( index != -1 ) {
        
        gGlobalFuncSymbolList.erase( gGlobalFuncSymbolList.begin() + index ) ;
        
        gGlobalSymbolList.push_back( symbolInfo ) ;
        
        cout << "New definition of " <<  symbolInfo.name << " entered ..." << endl ;
      } 
      
      
      else {
        
        gGlobalSymbolList.push_back( symbolInfo ) ;
        
        cout << "Definition of " <<  symbolInfo.name << " entered ..." << endl ;
      } 
    } 
  } 
  
  
  
  
  
  bool A_user_input( int &firstTokenStartLine, int &firstTokenStartColumn, bool &isQuit ) { 
    TOKENTYPE tokentype ; 
    int nextTokenIndex = -1 ;
    int index = 0 ;
    
    
    if ( !mIsCompute ) {
       
      
      PeekToken( tokentype ) ; 
       
      
      firstTokenStartLine = gPeekToken.tokenLine ;
      firstTokenStartColumn = gPeekToken.tokenColumn ;
      
      
      if ( tokentype == VOID || Type_specifier( tokentype ) ) {
        
        
        if ( Definition( nextTokenIndex ) ) return true ;
        else return false ; 
      } 
      
      
      else if ( tokentype == SEMICOLON  || tokentype == IDENTIFIER || tokentype == PP 
                || tokentype == MM || Sign( tokentype ) 
                || tokentype == CONSTANT_INT || tokentype == CONSTANT_FLOAT 
                || tokentype == CONSTANT_CHAR || tokentype == CONSTANT_STRING
                || tokentype == CONSTANT_BOOL || tokentype == LP || tokentype == LBC 
                || tokentype == IF || tokentype == WHILE
                || tokentype == RETURN || tokentype == DO ) {
        
        if ( tokentype == LBC ) mUserInputType = LOCALSTATEMENT ;
        else mUserInputType = STATEMENT ;
        
        
        if ( Statement( nextTokenIndex ) ) {
          
          
          index = FindTokenInAllTokenStringList( firstTokenStartLine, firstTokenStartColumn ) ;
          
          
          if ( gAllTokenStringList[index].tokentype == IDENTIFIER ) {
            
            
            if ( IsSystemSupportFunction( gAllTokenStringList[index].tokenID ) ) {
              
              
              if ( gAllTokenStringList[index].tokenID.compare( "ListAllVariables" )  == 0 ) {
                PrintListAllVariables() ;
                cout << "Statement executed ..." << endl ;
              } 
              
              
              else if ( gAllTokenStringList[index].tokenID.compare( "ListAllFunctions" )  == 0 ) {
                PrintListAllFunctions() ;
                cout << "Statement executed ..." << endl ;
              } 
              
              
              
              else if ( gAllTokenStringList[index].tokenID.compare( "ListVariable" )  == 0 && 
                        gAllTokenStringList[index+2].tokentype == CONSTANT_STRING ) {
                PrintListVariable( gAllTokenStringList[index+2].tokenID ) ;
                cout << "Statement executed ..." << endl ;          
              } 
              
              
              
              else if ( gAllTokenStringList[index].tokenID.compare( "ListFunction" )  == 0 && 
                        gAllTokenStringList[index+2].tokentype == CONSTANT_STRING ) {
                PrintListFunction( gAllTokenStringList[index+2].tokenID ) ;
                cout << "Statement executed ..." << endl ;            
              } 
              
              
              
              else if ( gAllTokenStringList[index].tokenID.compare( "Done" ) == 0
                        && gAllTokenStringList[index+2].tokentype == RP ) {
                isQuit = true ;          
              } 
              
              else {
                cout << "Statement executed ..." << endl ;
              } 
            } 
            
            else cout << "Statement executed ..." << endl ;
          } 
          
          else cout << "Statement executed ..." << endl ;
          
          return true ;
        } 
        else return false ; 
      } 
      
      else {
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        ThrowException( tokentype ) ;
        return false ;
      } 
    } 
    
    
    else {
      
    } 
    
    return false ;
  } 

   
  
  bool Definition( int &nextTokenIndex ) {
    TOKENTYPE tokentype ; 
    IndentifierSymbolInfo newDefineSymbol ; 

    
    if ( !mIsCompute ) {
      
      GetToken( tokentype ) ;
      gAllTokenStringList.push_back( gToken ) ;
      
      
      if ( tokentype == VOID ) {
         
        mUserInputType = DEFINEFUNCTION ; 
        
        
        
        mNewDefineFunc.funcSymbolReturnType = gToken.tokentype ; 
        mNewDefineFunc.funcStartLine = gToken.tokenLine ; 
        mNewDefineFunc.funcStartColumn = gToken.tokenColumn ; 
        
         
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        if ( tokentype == IDENTIFIER ) {
          
          
          mNewDefineFunc.name = gToken.tokenID ; 
          
          
          
          if ( Function_definition_without_ID( nextTokenIndex ) )  {
            
            
            
            mNewDefineFunc.funcEndLine = gAllTokenStringList[gAllTokenStringList.size()-1].tokenLine ;
            mNewDefineFunc.funcEndColumn = gAllTokenStringList[gAllTokenStringList.size()-1].tokenColumn ;

            
            InsertNewDenfineFunctionToGlobalFuncList( mNewDefineFunc ) ;
            
            
            return true ;  
          } 
          else return false ; 
        } 
        
        
        else {
          
          ThrowException( tokentype ) ;
          return false ;
        } 
      } 
      
      
      else if ( Type_specifier( tokentype ) ) {
        
        
        
        mNewDefineFunc.funcSymbolReturnType = gToken.tokentype ;
        newDefineSymbol.symbolReturnType = gToken.tokentype ;
        mNewDefineFunc.funcStartLine = gToken.tokenLine ; 
        mNewDefineFunc.funcStartColumn = gToken.tokenColumn ; 
        
        
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        if ( tokentype == IDENTIFIER ) {
          
          
          PeekToken( tokentype ) ;
          
          
          
          if ( tokentype == LP ) {
            
            mUserInputType = DEFINEFUNCTION ; 
            
            
            mNewDefineFunc.name = gToken.tokenID ; 
            
            
          } 
          
          
          else {
            mUserInputType = DEFINESYMBOL ;                           
            
            
            newDefineSymbol.name = gToken.tokenID ;                   
            newDefineSymbol.symbolStartLine = gToken.tokenLine ;   
            newDefineSymbol.symbolStartColumn = gToken.tokenColumn ; 
            mNewDefineSymbolList.push_back( newDefineSymbol ) ;      
            
          } 
          
          
          if ( Function_definition_or_declarators( nextTokenIndex ) ) {
            
            if ( mUserInputType == DEFINEFUNCTION ) {
              
              
              
              mNewDefineFunc.funcEndLine = gAllTokenStringList[gAllTokenStringList.size()-1].tokenLine ;
              mNewDefineFunc.funcEndColumn = gAllTokenStringList[gAllTokenStringList.size()-1].tokenColumn ;
              
              
              InsertNewDenfineFunctionToGlobalFuncList( mNewDefineFunc ) ;
              
              
            } 
            
            
            else {
              
              
              for ( int i = 0 ; i < mNewDefineSymbolList.size() ; i++ ) {
                InsertNewDenfineSymbolToGlobalSymbolList( mNewDefineSymbolList[i] ) ;
              } 
              
              mNewDefineSymbolList.clear() ;
            } 

            return true ;  
          } 
          else return false ; 
        } 
        
        
        else {
          
          ThrowException( tokentype ) ;
          return false ;
        } 
      } 
      
      
      else {    
        
        ThrowException( tokentype ) ;
        return false ;
      } 
    } 
    
    
    else {
      
    } 
    
    return false ;
  } 
  
  string CopyString( string token ) {
    string str = "" ;
    for ( int i = 0 ; i < token.size() ; i++ ) {
      str = str + token[i] ;
    } 
  
    return str ;
  } 
  
  bool Type_specifier( TOKENTYPE tokentype ) {
    if ( tokentype == INT || tokentype == FLOAT || tokentype == CHAR 
         || tokentype == STRING || tokentype == BOOL )
      return true ;
    else
      return false ;
  } 
  
  bool Function_definition_or_declarators( int &nextTokenIndex ) {
    TOKENTYPE tokentype ; 
    
    
    if ( !mIsCompute ) {
      
      PeekToken( tokentype ) ;
    } 
    
    
    else {
      
      PassTwoPeekToken( nextTokenIndex, tokentype ) ;
    } 
    
    
    
    if ( tokentype == LP ) {
      
      if ( Function_definition_without_ID( nextTokenIndex ) ) return true ;
      else return false ; 
    } 
    
    
    else if ( tokentype == LBK || tokentype == COMMA || tokentype == SEMICOLON )  {
      
      if ( Rest_of_declarators( nextTokenIndex ) ) return true ;
      else return false ; 
    } 
    
    
    else {
      
      
      if ( !mIsCompute ) {
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
      } 
      
      
      else {
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex = nextTokenIndex + 1 ;
      } 
      
      
      ThrowException( tokentype ) ;
      return false ;
    } 
    
    return false ;
  } 
  
  bool Rest_of_declarators( int &nextTokenIndex ) {
    TOKENTYPE tokentype ; 
    IndentifierSymbolInfo newDefineSymbol ; 
    int index = 0 ; 
    
    
    if ( !mIsCompute ) {
      
      PeekToken( tokentype ) ;
      
      
      if ( tokentype == LBK ) {
        
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        if ( tokentype == CONSTANT_INT || tokentype == CONSTANT_FLOAT 
             || tokentype == CONSTANT_CHAR || tokentype == CONSTANT_STRING || tokentype == CONSTANT_BOOL ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
          
          
          if ( tokentype == RBK ) {
            
            PeekToken( tokentype ) ;
            
            
            while ( tokentype == COMMA ) {
              
              
              GetToken( tokentype ) ;
              gAllTokenStringList.push_back( gToken ) ;
              
              
              
              GetToken( tokentype ) ;
              gAllTokenStringList.push_back( gToken ) ;
              
              
              if ( tokentype == IDENTIFIER ) {
                
                
                
                if ( mUserInputType == DEFINEFUNCTION ) {
                  
                  newDefineSymbol.name = gToken.tokenID ;   
                  
                  
                  newDefineSymbol.symbolReturnType = 
                  mNewDefineFunc.localSymbolList[mNewDefineFunc.localSymbolList.size()-1].symbolReturnType ;
                    
                  newDefineSymbol.symbolStartLine = gToken.tokenLine ;  
                  newDefineSymbol.symbolStartColumn = gToken.tokenColumn ; 
                  
                  
                  
                  index = FindTokenInSymbolList( gToken.tokenID, mNewDefineFunc.localSymbolList ) ; 
                  
                  
                  if ( index != -1 ) {
                    
                    mNewDefineFunc.localSymbolList.erase( mNewDefineFunc.localSymbolList.begin() + index ) ;
                    
                    
                    mNewDefineFunc.localSymbolList.push_back( newDefineSymbol ) ;
                    
                    IndentifierInit( newDefineSymbol ) ; 
                  } 
                  
                  
                  else {
                    
                    mNewDefineFunc.localSymbolList.push_back( newDefineSymbol ) ;
                    
                    IndentifierInit( newDefineSymbol ) ; 
                  } 
                } 
                
                
                
                else {
                  
                  newDefineSymbol.name = gToken.tokenID ; 
                  newDefineSymbol.symbolStartLine = gToken.tokenLine ; 
                  newDefineSymbol.symbolStartColumn = gToken.tokenColumn ; 
                
                  
                  newDefineSymbol.symbolReturnType = 
                  mNewDefineSymbolList[mNewDefineSymbolList.size()-1].symbolReturnType ; 
                
                  mNewDefineSymbolList.push_back( newDefineSymbol ) ; 

                } 
                
                
                
                
                PeekToken( tokentype ) ;
                
                
                if ( tokentype == LBK ) {
                  
                  
                  GetToken( tokentype ) ;
                  gAllTokenStringList.push_back( gToken ) ;
                  
                  
                  
                  GetToken( tokentype ) ;
                  gAllTokenStringList.push_back( gToken ) ;
                  
                  
                  if ( tokentype == CONSTANT_INT || tokentype == CONSTANT_FLOAT 
                       || tokentype == CONSTANT_CHAR || tokentype == CONSTANT_STRING 
                       || tokentype == CONSTANT_BOOL ) {
                    
                    
                    
                    GetToken( tokentype ) ;
                    gAllTokenStringList.push_back( gToken ) ; 
                    
                    
                    if ( tokentype == RBK ) {
                      
                      PeekToken( tokentype ) ;
                    } 
                    
                    
                    else {
                      
                      ThrowException( tokentype ) ;
                      return false ;
                    } 
                  } 
                  
                  
                  else {
                    
                    ThrowException( tokentype ) ;
                    return false ;
                  } 
                } 
              } 
              
              
              else {
                
                ThrowException( tokentype ) ;
                return false ;  
              } 
            } 
            
            
            GetToken( tokentype ) ;
            gAllTokenStringList.push_back( gToken ) ;
            
            
            if ( tokentype == SEMICOLON ) {
              return true;
            } 
            
            else {
              
              ThrowException( tokentype ) ;
              return false ;
            } 
          } 
          
          
          else {
            
            ThrowException( tokentype ) ;
            return false ;
          } 
        } 
        
        
        else {
          
          ThrowException( tokentype ) ;
          return false ;
        } 
      } 
      
      else if ( tokentype == COMMA ) {
        
        while ( tokentype == COMMA ) {
          
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
          
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
          
          
          if ( tokentype == IDENTIFIER ) {
            
            
            
            if ( mUserInputType == DEFINEFUNCTION ) {
              
              newDefineSymbol.name = gToken.tokenID ;   
              
              
              newDefineSymbol.symbolReturnType = 
              mNewDefineFunc.localSymbolList[mNewDefineFunc.localSymbolList.size()-1].symbolReturnType ;
                
              newDefineSymbol.symbolStartLine = gToken.tokenLine ;  
              newDefineSymbol.symbolStartColumn = gToken.tokenColumn ; 
              
              
              
              index = FindTokenInSymbolList( gToken.tokenID, mNewDefineFunc.localSymbolList ) ; 
              
              
              if ( index != -1 ) {
                
                mNewDefineFunc.localSymbolList.erase( mNewDefineFunc.localSymbolList.begin() + index ) ;
                
                
                mNewDefineFunc.localSymbolList.push_back( newDefineSymbol ) ;
                
                IndentifierInit( newDefineSymbol ) ; 
              } 
              
              
              else {
                
                mNewDefineFunc.localSymbolList.push_back( newDefineSymbol ) ;
                
                IndentifierInit( newDefineSymbol ) ; 
              } 
            } 
            
            
            
            else {
              
              newDefineSymbol.name = gToken.tokenID ; 
              newDefineSymbol.symbolStartLine = gToken.tokenLine ; 
              newDefineSymbol.symbolStartColumn = gToken.tokenColumn ; 
              
              
              
              newDefineSymbol.symbolReturnType = 
              mNewDefineSymbolList[mNewDefineSymbolList.size()-1].symbolReturnType ; 
              
              mNewDefineSymbolList.push_back( newDefineSymbol ) ; 

            } 
            
            
            
            
            PeekToken( tokentype ) ;
            
            
            if ( tokentype == LBK ) {
              
              
              
              GetToken( tokentype ) ;
              gAllTokenStringList.push_back( gToken ) ;
              
              
              
              GetToken( tokentype ) ;
              gAllTokenStringList.push_back( gToken ) ;
              
              
              if ( tokentype == CONSTANT_INT || tokentype == CONSTANT_FLOAT 
                   || tokentype == CONSTANT_CHAR || tokentype == CONSTANT_STRING 
                   || tokentype == CONSTANT_BOOL ) {
                
                
                
                GetToken( tokentype ) ;
                gAllTokenStringList.push_back( gToken ) ; 
                
                
                if ( tokentype == RBK ) {
                  
                  PeekToken( tokentype ) ;
                } 
                
                
                else {
                  
                  ThrowException( tokentype ) ;
                  return false ;
                } 
              } 
              
              
              else {
                
                ThrowException( tokentype ) ;
                return false ;
              } 
            } 
          } 
          
          
          else {
            
            ThrowException( tokentype ) ;
            return false ;
          } 
        } 
        
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        if ( tokentype == SEMICOLON ) {
          return true ;
        } 
        
        else {
          
          ThrowException( tokentype ) ;
          return false ;
        } 
      } 
      
      
      else {
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        if ( tokentype == SEMICOLON ) return true ;
        
        
        else {
          
          ThrowException( tokentype ) ;
          return false ;
        } 
      } 
    } 
    
    
    else {
    
    } 
    
    return false ;
  } 
  
  bool Function_definition_without_ID( int &nextTokenIndex ) {
    TOKENTYPE tokentype ; 
    
    
    if ( !mIsCompute ) {
      
      
      GetToken( tokentype ) ;
      gAllTokenStringList.push_back( gToken ) ;
      
      
      if ( tokentype == LP ) {
        
        PeekToken( tokentype ) ;
        
        
        if ( tokentype == VOID || Type_specifier( tokentype ) ) {
          if ( tokentype == VOID ) {
            
            GetToken( tokentype ) ;
            gAllTokenStringList.push_back( gToken ) ;
          } 
          
          else {
            
            
            if ( !Formal_parameter_list( nextTokenIndex ) ) {
              return false ;
            } 
            
          } 
        } 
        
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        if ( tokentype == RP ) {
          
          if ( Compound_statement( nextTokenIndex ) ) return true ;
          else return false ; 
        } 
        
        
        else {
          
          ThrowException( tokentype ) ;
          return false ;
        } 
      } 
      
      
      else {
        
        ThrowException( tokentype ) ;
        return false ;
      } 
    } 
    
    
    else {
      
    } 
    
    return false ;
  } 
  
  bool Formal_parameter_list( int &nextTokenIndex ) {
    TOKENTYPE tokentype ; 
    IndentifierSymbolInfo newDefineSymbol ;
    ArgsDefineInfo argDefineInfo ;
    
    
    if ( !mIsCompute ) {
      
      
      
      GetToken( tokentype ) ;
      gAllTokenStringList.push_back( gToken ) ;
      
      
      if ( Type_specifier( tokentype ) ) {
        
        
        argDefineInfo.argReturnType = tokentype ; 
        
        
        
        PeekToken( tokentype ) ;
        
        
        if ( tokentype == LAND ) {
          
          argDefineInfo.isReturn = true ; 
          
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
        } 
        
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        if ( tokentype == IDENTIFIER ) {
          
          
          argDefineInfo.name = gToken.tokenID ; 
          
          newDefineSymbol.name =  argDefineInfo.name ;                    
          
          newDefineSymbol.symbolReturnType = argDefineInfo.argReturnType ;  
          
          newDefineSymbol.symbolStartLine = gToken.tokenLine ;              
          
          newDefineSymbol.symbolStartColumn = gToken.tokenColumn ;          
          
          mNewDefineFunc.localSymbolList.push_back( newDefineSymbol ) ;     
          
          mNewDefineFunc.argDefineList.push_back( argDefineInfo ) ;         
          IndentifierInit( newDefineSymbol ) ; 
          argDefineInfo.isReturn = false ; 
          
          
          
          PeekToken( tokentype ) ;
          
          if ( tokentype == LBK ) {
            
            
            GetToken( tokentype ) ;
            gAllTokenStringList.push_back( gToken ) ;
            
            
            GetToken( tokentype ) ;
            gAllTokenStringList.push_back( gToken ) ;
            
            
            if ( tokentype == CONSTANT_INT || tokentype == CONSTANT_CHAR 
                 || tokentype == CONSTANT_FLOAT || tokentype == CONSTANT_BOOL 
                 || tokentype == CONSTANT_STRING ) {
              
              
              GetToken( tokentype ) ;
              gAllTokenStringList.push_back( gToken ) ;
              
              
              if ( tokentype == RBK ) {
                
                PeekToken( tokentype ) ;
              } 
              
              
              else {
                
                ThrowException( tokentype ) ;
                return false ;
              } 
            } 
            
            
            else {
              
              ThrowException( tokentype ) ;
              return false ;
            } 
          } 
          
          
          while ( tokentype == COMMA ) {
            
            
            GetToken( tokentype ) ;
            gAllTokenStringList.push_back( gToken ) ;
            
            
            
            GetToken( tokentype ) ;
            gAllTokenStringList.push_back( gToken ) ;
            
            
            if ( Type_specifier( tokentype ) ) {
              
              
              argDefineInfo.argReturnType = tokentype ; 
              
              
              
              PeekToken( tokentype ) ;
              
              
              if ( tokentype == LAND ) {
                
                argDefineInfo.isReturn = true ; 
                
          
                
                GetToken( tokentype ) ;
                gAllTokenStringList.push_back( gToken ) ;
              } 
              
              
              GetToken( tokentype ) ;
              gAllTokenStringList.push_back( gToken ) ;
              
              
              if ( tokentype == IDENTIFIER ) {
                
                
                argDefineInfo.name = gToken.tokenID ; 
                
                newDefineSymbol.name =  argDefineInfo.name ;                    
                
                newDefineSymbol.symbolReturnType = argDefineInfo.argReturnType ;  
                
                newDefineSymbol.symbolStartLine = gToken.tokenLine ;             
                
                newDefineSymbol.symbolStartColumn = gToken.tokenColumn ;          
                
                mNewDefineFunc.localSymbolList.push_back( newDefineSymbol ) ;     
                
                mNewDefineFunc.argDefineList.push_back( argDefineInfo ) ;         
                IndentifierInit( newDefineSymbol ) ; 
                argDefineInfo.isReturn = false ; 
                
                
                
                PeekToken( tokentype ) ;
                
                if ( tokentype == LBK ) {
                  
                  
                  GetToken( tokentype ) ;
                  gAllTokenStringList.push_back( gToken ) ;
            
                  
                  GetToken( tokentype ) ;
                  gAllTokenStringList.push_back( gToken ) ;
            
                  
                  if ( tokentype == CONSTANT_INT || tokentype == CONSTANT_CHAR 
                       || tokentype == CONSTANT_FLOAT || tokentype == CONSTANT_BOOL 
                       || tokentype == CONSTANT_STRING ) {
                    
                    
                    GetToken( tokentype ) ;
                    gAllTokenStringList.push_back( gToken ) ;
              
                    
                    if ( tokentype == RBK ) {
                      
                      PeekToken( tokentype ) ;
                    } 
              
                    
                    else {
                      
                      ThrowException( tokentype ) ;
                      return false ;
                    } 
                  } 
            
                  
                  else {
                    
                    ThrowException( tokentype ) ;
                    return false ;
                  } 
                } 
              } 
              
              
              else {
                
                ThrowException( tokentype ) ;
                return false ;
              } 
            } 
            
            
            else {
              
              ThrowException( tokentype ) ;
              return false ;
            } 
          } 
          
          return true ;   
        } 
        
        
        else {
          
          ThrowException( tokentype ) ;
          return false ;
        } 
      } 
      
      
      else {
        
        ThrowException( tokentype ) ;
        return false ;
      } 
    } 
    
    
    else {
      
    } 
    
    return false ;
  } 
  
  bool Compound_statement( int &nextTokenIndex ) {
    TOKENTYPE tokentype ; 
    
    
    if ( !mIsCompute ) {
      
      GetToken( tokentype ) ;
      gAllTokenStringList.push_back( gToken ) ;
      
      
      if ( tokentype == LBC ) {
        
        PeekToken( tokentype ) ;
        
        while ( Type_specifier( tokentype ) || tokentype == SEMICOLON  
                || tokentype == IDENTIFIER || tokentype == PP || tokentype == MM 
                || Sign( tokentype ) || tokentype == CONSTANT_INT 
                || tokentype == CONSTANT_FLOAT || tokentype == CONSTANT_CHAR 
                || tokentype == CONSTANT_STRING || tokentype == CONSTANT_BOOL || tokentype == LP
                || tokentype == RETURN || tokentype == LBC || tokentype == IF || tokentype == WHILE 
                || tokentype == DO ) {
                  
          
          if ( Type_specifier( tokentype ) ) {
            
            if ( Declaration( nextTokenIndex ) ) {
              
              PeekToken( tokentype ) ;
            } 
            
            else return false ; 
          } 
          
          else {
            
            if ( Statement( nextTokenIndex ) ) {
              
              PeekToken( tokentype ) ;
            } 
          
            else return false ; 
          } 
        } 

        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        if ( tokentype == RBC ) {
          return true ;
        } 
        
        
        else {
          
          ThrowException( tokentype ) ;
          return false ;
        } 
      } 
      
      
      else {
        
        ThrowException( tokentype ) ;
        return false ;
      } 
    } 
    
    
    else {
      
    } 
    
    return false ;
  } 
  
  bool Declaration( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
    IndentifierSymbolInfo newDefineSymbol ;
    int index = 0 ;
    
    
    if ( !mIsCompute ) {
      
      GetToken( tokentype ) ;
      gAllTokenStringList.push_back( gToken ) ;
      
      
      if ( Type_specifier( tokentype ) ) {
        
        
        
        newDefineSymbol.symbolReturnType = tokentype ;
        

        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        if ( tokentype == IDENTIFIER ) {
          
          
          
          if ( mUserInputType == DEFINEFUNCTION ) {
            
            newDefineSymbol.name = gToken.tokenID ;   
            newDefineSymbol.symbolStartLine = gToken.tokenLine ;  
            newDefineSymbol.symbolStartColumn = gToken.tokenColumn ; 
            
            
            index = FindTokenInSymbolList( gToken.tokenID, mNewDefineFunc.localSymbolList ) ; 
            
            
            if ( index != -1 ) {
              
              mNewDefineFunc.localSymbolList.erase( mNewDefineFunc.localSymbolList.begin() + index ) ;
              
              
              mNewDefineFunc.localSymbolList.push_back( newDefineSymbol ) ;
              
              IndentifierInit( newDefineSymbol ) ; 
            } 
            
            
            else {
              
              mNewDefineFunc.localSymbolList.push_back( newDefineSymbol ) ;
              
              IndentifierInit( newDefineSymbol ) ; 
            } 
          } 
          
          
          
          else {
            
            newDefineSymbol.name = gToken.tokenID ; 
            newDefineSymbol.symbolStartLine = gToken.tokenLine ; 
            newDefineSymbol.symbolStartColumn = gToken.tokenColumn ; 
            mNewDefineSymbolList.push_back( newDefineSymbol ) ; 
            IndentifierInit( newDefineSymbol ) ; 
            
          } 
          
                
          
          if ( Rest_of_declarators( nextTokenIndex ) ) {
            
            
            if ( mUserInputType == DEFINEFUNCTION ) {
              for ( int i = 0 ; i < mNewDefineSymbolList.size() ; i++ ) {
                index = FindTokenInSymbolList( mNewDefineSymbolList[i].name, 
                                               mNewDefineFunc.localSymbolList ) ;
                
                
                if ( index != -1 ) {
                  
                  mNewDefineFunc.localSymbolList.erase( mNewDefineFunc.localSymbolList.begin() + index ) ;
                  
                  
                  mNewDefineFunc.localSymbolList.push_back( newDefineSymbol ) ;
                } 
                
                
                else {
                  
                  mNewDefineFunc.localSymbolList.push_back( newDefineSymbol ) ;
                } 
              } 
              
              mNewDefineSymbolList.clear() ; 
            } 
            
            else {
              
              if ( mUserInputType == DEFINESYMBOL ) {
                
                
                for ( int i = 0 ; i < mNewDefineSymbolList.size() ; i++ ) {
                  InsertNewDenfineSymbolToGlobalSymbolList( mNewDefineSymbolList[i] ) ;
                } 
                
                mNewDefineSymbolList.clear() ; 
              } 
              
              
              else {
                for ( int i = 0 ; i < mNewDefineSymbolList.size() ; i++ ) {
                  index = FindTokenInSymbolList( mNewDefineSymbolList[i].name, 
                                                 mStmtNewDefineSymbolList ) ;
                  
                  if ( index != -1 ) {
                    
                    mStmtNewDefineSymbolList.erase( mStmtNewDefineSymbolList.begin() + index ) ;
                    
                    
                    mStmtNewDefineSymbolList.push_back( newDefineSymbol ) ;
                  } 
                  
                  
                  else {
                    
                    mStmtNewDefineSymbolList.push_back( newDefineSymbol ) ;
                  } 
                } 
                
                mNewDefineSymbolList.clear() ; 
              } 
            } 
            
            
            return true ;
          } 
          else return false ; 
        } 
        
        
        else {
          
          ThrowException( tokentype ) ;
          return false ;
        } 
      } 
      
      
      else {
        
        ThrowException( tokentype ) ;
        return false ;
      } 
    } 
    
    
    else {
      
    } 
    
    return false ;
  } 
  
  bool Statement( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
  
    
    
    if ( !mIsCompute ) {
      
      PeekToken( tokentype ) ;
      
      
      if ( tokentype == SEMICOLON ) {
        
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        return true ;
      } 
      
      
      else if ( tokentype == IDENTIFIER || tokentype == PP || tokentype == MM 
                || Sign( tokentype ) || tokentype == CONSTANT_INT 
                || tokentype == CONSTANT_FLOAT || tokentype == CONSTANT_CHAR 
                || tokentype == CONSTANT_STRING || tokentype == CONSTANT_BOOL
                || tokentype == LP ) {
        
        
        if ( Expression( nextTokenIndex ) ) {
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
          
          
          if ( tokentype == SEMICOLON ) {
            return true ;
          } 
          
          
          else {
            
            ThrowException( tokentype ) ;
            return false ;
          } 
        } 
        
        else return false ;   
      } 
      
      
      else if ( tokentype == RETURN ) {
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        PeekToken( tokentype ) ;
        
        
        if ( tokentype == IDENTIFIER || tokentype == PP || tokentype == MM 
             ||  Sign( tokentype ) || tokentype == CONSTANT_INT 
             || tokentype == CONSTANT_FLOAT || tokentype == CONSTANT_CHAR 
             || tokentype == CONSTANT_STRING || tokentype == CONSTANT_BOOL
             || tokentype == LP ) {
          
          
          if ( Expression( nextTokenIndex ) ) {
            
            GetToken( tokentype ) ;
            gAllTokenStringList.push_back( gToken ) ;
            
            
            if ( tokentype == SEMICOLON ) {
              return true ;
            } 
          
            
            else {
              
              ThrowException( tokentype ) ;
              return false ;
            } 
          } 
          
          
          else return false ; 
        } 
      } 
      
      
      else if ( tokentype == LBC ) {
        
        if ( Compound_statement( nextTokenIndex ) ) return true ;
        else return false ; 
      } 
      
      
      else if ( tokentype == IF ) {
        
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        if ( tokentype == LP ) {
          
          
          if ( Expression( nextTokenIndex ) ) {
            
            GetToken( tokentype ) ;
            gAllTokenStringList.push_back( gToken ) ;
            
            
            if ( tokentype == RP ) {
              
              if ( Statement( nextTokenIndex ) ) {
                
                PeekToken( tokentype ) ;
                
                if ( tokentype == ELSE ) {
                  
                  
                  GetToken( tokentype ) ;
                  gAllTokenStringList.push_back( gToken ) ;
                  
                  
                  if ( Statement( nextTokenIndex ) ) return true ;
                  else return false ; 
                } 
                
                return true ;
              } 
              
              else return false ; 
            } 
            
            
            else {
              
              ThrowException( tokentype ) ;
              return false ;
            } 
        
          } 
          
          else return false ; 
        } 
        
        
        else {
          
          ThrowException( tokentype ) ;
          return false ;
        } 
      } 
      
      
      else if ( tokentype == WHILE ) {
        
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        if ( tokentype == LP ) {
          
          if ( Expression( nextTokenIndex ) ) {
            
            GetToken( tokentype ) ;
            gAllTokenStringList.push_back( gToken ) ;
            
            
            if ( tokentype == RP ) {
              
              if ( Statement( nextTokenIndex ) ) return true ;
              else return false ; 
            } 
            
            
            else {
              
              ThrowException( tokentype ) ;
              return false ;
            } 
        
          } 
          
          else return false ; 
        } 
        
        
        else {
          
          ThrowException( tokentype ) ;
          return false ;
        } 
      } 
      
      
      else if ( tokentype == DO ) {
        
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        if ( Statement( nextTokenIndex ) ) {
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
          
          
          if ( tokentype == WHILE ) {
            
            GetToken( tokentype ) ;
            gAllTokenStringList.push_back( gToken ) ;
            
            
            if ( tokentype == LP ) {
              
              if ( Expression( nextTokenIndex ) ) {
                
                GetToken( tokentype ) ;
                gAllTokenStringList.push_back( gToken ) ;
                
                
                if ( tokentype == RP ) {
                  
                  GetToken( tokentype ) ;
                  gAllTokenStringList.push_back( gToken ) ;
                  
                  
                  if ( tokentype == SEMICOLON ) {
                    return true ;
                  } 
                  
                  
                  else {
                    
                    ThrowException( tokentype ) ;
                    return false ;
                  } 
                } 
                
                
                else {
                  
                  ThrowException( tokentype ) ;
                  return false ;
                } 
              } 
              
              
              else return false ;
            } 
            
            
            else {
              
              ThrowException( tokentype ) ;
              return false ;
            } 
          } 
          
          
          else {
            
            ThrowException( tokentype ) ;
            return false ;
          } 
        } 
        
        else return false ; 
      } 
      
      
      else {
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        ThrowException( tokentype ) ;
        return false ;
      } 
    } 
    
    
    else {
      
    } 
    
    return false ;
  } 
  
  bool Expression( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
    
    
    if ( !mIsCompute ) {
      
      
      if ( Basic_expression( nextTokenIndex ) ) {
        
        PeekToken( tokentype ) ;
        
        
        
        while ( tokentype == COMMA ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
          
          
          if ( Basic_expression( nextTokenIndex ) ) {
            
            PeekToken( tokentype ) ;
          } 
          
          else return false ; 
        } 
        
        return true ;
      } 
      
      else return false ; 
    } 
    
    
    else {
      
    } 
    
    return false ;
  } 
  
  
  bool IsSystemSupportFunction( string str ) {
    
    if ( str.compare( "ListAllVariables" ) == 0 || str.compare( "ListAllFunctions" ) == 0
         || str.compare( "ListVariable" ) == 0 || str.compare( "ListFunction" ) == 0
         || str.compare( "Done" ) == 0 || str.compare( "cout" ) == 0 || str.compare( "cin" ) == 0 ) {
      return true ;
    } 
    
    return false ;
  } 
  
  bool Basic_expression( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
    IndentifierSymbolInfo newDefineSymbol ;
    int index = 0 ;
    
    
    if ( !mIsCompute ) {
      
      
      GetToken( tokentype ) ;
      gAllTokenStringList.push_back( gToken ) ;
      
      
      if ( tokentype == IDENTIFIER ) {
        
        
        
        if ( !IsSystemSupportFunction( gToken.tokenID ) ) {
          
          
          if ( mUserInputType == DEFINEFUNCTION ) {
            
            index = FindTokenInSymbolList( gToken.tokenID, mNewDefineFunc.localSymbolList ) ;
            
            
            if ( index == -1 ) {
              
              index = FindTokenInSymbolList( gToken.tokenID, gGlobalSymbolList ) ;
              
              
              if ( index == -1 ) {
                
                index = FindTokenInFunctionList( gToken.tokenID ) ;
                
                
                if ( index == -1 ) {
                  
                  throw new Exception3( gToken.tokenID ) ;
                  return false ;
                } 
              } 
            } 
          } 
          
          
          else {
            if (  mUserInputType == LOCALSTATEMENT ) {
              
              index = FindTokenInSymbolList( gToken.tokenID, mStmtNewDefineSymbolList ) ;
              
              
              if ( index == -1 ) {
                
                
                index = FindTokenInSymbolList( gToken.tokenID, gGlobalSymbolList ) ;
                
                
                if ( index == -1 ) {
                  
                  index = FindTokenInFunctionList( gToken.tokenID ) ;
                  
                  
                  if ( index == -1 ) {
                    
                    throw new Exception3( gToken.tokenID ) ;
                    return false ;
                  } 
                } 
              } 
            } 
            
            else {
              
              index = FindTokenInSymbolList( gToken.tokenID, gGlobalSymbolList ) ;
              
              
              if ( index == -1 ) {
                
                index = FindTokenInFunctionList( gToken.tokenID ) ;
                
                
                if ( index == -1 ) {
                  
                  throw new Exception3( gToken.tokenID ) ;
                  return false ;
                } 
              } 
            } 
          } 
        } 
        
          
        
        if ( Rest_of_Identifier_started_basic_exp( nextTokenIndex ) ) return true ;
        else return false ; 
      } 
      
      
      else if ( tokentype == PP || tokentype == MM ) {
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        if ( tokentype == IDENTIFIER ) {
          
          
          
          if ( !IsSystemSupportFunction( gToken.tokenID ) ) {
            
            if ( mUserInputType == DEFINEFUNCTION ) {
              
              index = FindTokenInSymbolList( gToken.tokenID, mNewDefineFunc.localSymbolList ) ;
              
              
              if ( index == -1 ) {
                
                index = FindTokenInSymbolList( gToken.tokenID, gGlobalSymbolList ) ;
                
                
                if ( index == -1 ) {
                  
                  index = FindTokenInFunctionList( gToken.tokenID ) ;
                  
                  
                  if ( index == -1 ) {
                    
                    throw new Exception3( gToken.tokenID ) ;
                    return false ;
                  } 
                } 
              } 
            } 
            
            
            else {
              
              if ( mUserInputType == LOCALSTATEMENT ) {
                
                index = FindTokenInSymbolList( gToken.tokenID, mStmtNewDefineSymbolList ) ;
                
                
                if ( index == -1 ) {
                  
                  
                  index = FindTokenInSymbolList( gToken.tokenID, gGlobalSymbolList ) ;
                  
                  
                  if ( index == -1 ) {
                    
                    index = FindTokenInFunctionList( gToken.tokenID ) ;
                    
                    
                    if ( index == -1 ) {
                      
                      throw new Exception3( gToken.tokenID ) ;
                      return false ;
                    } 
                  } 
                } 
              } 
              
              else {
                
                index = FindTokenInSymbolList( gToken.tokenID, gGlobalSymbolList ) ;
                
                
                if ( index == -1 ) {
                  
                  index = FindTokenInFunctionList( gToken.tokenID ) ;
                  
                  
                  if ( index == -1 ) {
                    
                    throw new Exception3( gToken.tokenID ) ;
                    return false ;
                  } 
                } 
              } 
              
            } 
          } 
          
          
          
          if ( Rest_of_PPMM_Identifier_started_basic_exp( nextTokenIndex ) ) return true ;
          else return false ; 
        } 
        
        
        else {
          
          ThrowException( tokentype ) ;
          return false ;
        } 
      } 
      
      
      else if ( Sign( tokentype ) ) {
        
        PeekToken( tokentype ) ;
        
        while ( Sign( tokentype ) ) {
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
          
          
          PeekToken( tokentype ) ;
        } 
        
        
        if ( Signed_unary_exp( nextTokenIndex ) ) {
          
          if ( Romce_and_romloe( nextTokenIndex ) ) return true ;
          else return false ; 
        } 
        
        else return false ; 
      } 
      
      
      else if ( tokentype == CONSTANT_INT || tokentype == CONSTANT_FLOAT 
                || tokentype == CONSTANT_CHAR || tokentype == CONSTANT_STRING 
                || tokentype == CONSTANT_BOOL ) {
        
        if ( Romce_and_romloe( nextTokenIndex ) ) return true ;
        else return false ; 
      } 
      
      
      else if ( tokentype == LP ) {
        
        if ( Expression( nextTokenIndex ) ) {
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
          
          
          if ( tokentype == RP ) {
            
            if ( Romce_and_romloe( nextTokenIndex ) ) return true ;
            else return false ; 
          } 
          
          
          else {
            
            ThrowException( tokentype ) ;
            return false ;
          } 
        } 
        
        else return false ; 
      } 
      
      
      else {
        
        ThrowException( tokentype ) ;
        return false ;
      } 
    } 
    
    
    else {
      
    } 
    
    return false ;
  } 
  
  bool Rest_of_Identifier_started_basic_exp( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
    
    
    if ( !mIsCompute ) {
      
      PeekToken( tokentype ) ;
      
      if ( tokentype == LBK ) {
        
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        if ( Expression( nextTokenIndex ) ) {
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
          
          
          if ( tokentype == RBK ) {
            
            PeekToken( tokentype ) ;
            
            if ( Assignment_operator( tokentype ) ) {
              
              GetToken( tokentype ) ;
              gAllTokenStringList.push_back( gToken ) ;
              
              
              if ( Basic_expression( nextTokenIndex ) ) return true ;
              else return false ; 
            } 
            
            else if ( tokentype == PP ||  tokentype == MM ) {
              
              
              GetToken( tokentype ) ;
              gAllTokenStringList.push_back( gToken ) ;
              
              
              if ( Romce_and_romloe( nextTokenIndex ) ) return true ;
              else return false ; 
            } 
            
            else {
              
              if ( Romce_and_romloe( nextTokenIndex ) ) return true ;
              else return false ; 
            } 
          } 
          
          
          else {
            
            ThrowException( tokentype ) ;
            return false ;
          } 
        } 
        
        else return false ; 
      } 
      
      else if ( Assignment_operator( tokentype ) ) { 
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        if ( Basic_expression( nextTokenIndex ) ) return true ;
        else return false ; 
      } 
      
      else if ( tokentype == PP || tokentype == MM ) {
        
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        if ( Romce_and_romloe( nextTokenIndex ) ) return true ;
        else return false ; 
      } 
      
      else if ( tokentype == LP ) {
        
        
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        PeekToken( tokentype ) ;
        
        
        if ( tokentype == IDENTIFIER || tokentype == PP || tokentype == MM 
             || Sign( tokentype ) || tokentype == CONSTANT_INT
             || tokentype == CONSTANT_FLOAT || tokentype == CONSTANT_CHAR 
             || tokentype == CONSTANT_STRING || tokentype == CONSTANT_BOOL 
             || tokentype == LP ) {
          
          
          if ( !Actual_parameter_list( nextTokenIndex ) ) {
            return false ;
          } 
        } 
        
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        if ( tokentype == RP ) {
          
          if ( Romce_and_romloe( nextTokenIndex ) ) return true ;
          else return false ; 
        } 
        
        
        else {
          
          ThrowException( tokentype ) ;
          return false ;
        } 
      } 
      
      
      else {
        
        if ( Romce_and_romloe( nextTokenIndex ) ) return true ;
        else return false ; 
      } 
    } 
    
    
    else {
      
    } 
    
    return false ;
  } 
  
  bool Rest_of_PPMM_Identifier_started_basic_exp( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
    
    
    if ( !mIsCompute ) {
      
      PeekToken( tokentype ) ;
      
      if ( tokentype == LBK ) {
        
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ; 
        
        
        if ( Expression( nextTokenIndex ) ) {
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ; 
          
          
          if ( tokentype != RBK ) {
            
            ThrowException( tokentype ) ;
            return false ;
          } 
        } 
        
        else return false ; 
      } 
      
      
      if ( Romce_and_romloe( nextTokenIndex ) ) return true ;
      else return false ; 
    } 
    
    
    else {
      
    } 
    
    return false ;
  } 
  
  bool Sign( TOKENTYPE tokentype ) {
    if ( tokentype == PLUS || tokentype == MINUS || tokentype == NOT ) return true ;
    return false ;
  } 
   
  bool Actual_parameter_list( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
  

    
    if ( Basic_expression( nextTokenIndex ) ) {
      
      
      if ( !mIsCompute ) {
        
        PeekToken( tokentype ) ;
      } 
      
      
      else {
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      } 
      
      while ( tokentype == COMMA ) {
        
        
        if ( !mIsCompute ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
        } 
        
        
        else {
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex = nextTokenIndex + 1 ;  
        } 
        
        
        if ( Basic_expression( nextTokenIndex ) ) {
          
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      return true ;
    } 

    return false ; 
  } 
  
  bool Assignment_operator( TOKENTYPE tokentype ) {
    
    if ( tokentype == ASSIGN || tokentype == TE || tokentype == DE 
         || tokentype == RE || tokentype == PE || tokentype == ME ) 
      return true ;
      
    return false ;
  } 
  
  bool Romce_and_romloe( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
    
    
    if ( Rest_of_maybe_logical_OR_exp( nextTokenIndex ) ) {
      
      
      if ( !mIsCompute ) {
        
        PeekToken( tokentype ) ;
      } 
      
      
      else {
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      } 
      
      
      if ( tokentype == QMARK ) {
        
        if ( !mIsCompute ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
        } 
        
        
        else {
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex = nextTokenIndex + 1 ; 
        } 
        
        
        if ( Basic_expression( nextTokenIndex ) ) {
          
          if ( !mIsCompute ) {
            
            GetToken( tokentype ) ;
            gAllTokenStringList.push_back( gToken ) ;
          } 
          
          
          else {
            PassTwoGetToken( nextTokenIndex, tokentype ) ;
            nextTokenIndex = nextTokenIndex + 1 ; 
          } 
          
          
          if ( tokentype == COLON ) {
            
            if ( Basic_expression( nextTokenIndex ) ) return true ;
            else return false ; 
          } 
          
          
          else {
            
            ThrowException( tokentype ) ;
            return false ;
          } 
        } 
        
        else return false ; 
      } 
      
      return true ;
    } 
    
    return false ; 
  } 
  
  bool Rest_of_maybe_logical_OR_exp( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
    
    
    if ( Rest_of_maybe_logical_AND_exp( nextTokenIndex ) ) {
      
      
      if ( !mIsCompute ) {
        
        PeekToken( tokentype ) ;
      } 
      
      
      else {
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      } 
      
      while ( tokentype == OR ) {
        
        
        if ( !mIsCompute ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
        } 
        
        
        else {
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex = nextTokenIndex + 1 ; 
        } 
        
        
        if ( Maybe_logical_AND_exp( nextTokenIndex ) ) {
          
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      return true ;
    } 
    
    return false ; 
  } 
   
  bool Maybe_logical_AND_exp( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
    
    
    if ( Maybe_bit_OR_exp( nextTokenIndex ) ) {
      
      
      if ( !mIsCompute ) {
        
        PeekToken( tokentype ) ;
      } 
      
      
      else {
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      } 
      
      while ( tokentype == AND ) {
        
        
        if ( !mIsCompute ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
        } 
        
        
        else {
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex = nextTokenIndex + 1 ; 
        } 
      
        
        if ( Maybe_bit_OR_exp( nextTokenIndex ) ) {
          
          if ( !mIsCompute  ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      return true ;
    } 
    
    return false ; 
  } 
  
  bool Rest_of_maybe_logical_AND_exp( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
    
    
    if ( Rest_of_maybe_bit_OR_exp( nextTokenIndex ) ) {
      
      if ( !mIsCompute ) {
        
        PeekToken( tokentype ) ;
      } 
      
      
      else {
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      } 
    
      while ( tokentype == AND ) {
        
        if ( !mIsCompute ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
        } 
        
        
        else {
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex = nextTokenIndex + 1 ;
        } 
        
        
        if ( Maybe_bit_OR_exp( nextTokenIndex ) ) {
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      return true ;
    } 
    
    return false ; 
  } 
   
  bool Maybe_bit_OR_exp( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
    
    
    if ( Maybe_bit_ex_OR_exp( nextTokenIndex ) ) {
      
      if ( !mIsCompute ) {
        
        PeekToken( tokentype ) ;
      } 
      
      
      else {
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      } 
      
      while ( tokentype == LOR ) {
        
        if ( !mIsCompute ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
        } 
        
        
        else {
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex = nextTokenIndex + 1 ;
        } 
        
        
        if ( Maybe_bit_ex_OR_exp( nextTokenIndex ) ) {
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      return true ;
    } 
    
    return false ; 
  } 
  
  bool Rest_of_maybe_bit_OR_exp( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
    
    
    if ( Rest_of_maybe_bit_ex_OR_exp( nextTokenIndex ) ) {
      
      
      if ( !mIsCompute ) {
        
        PeekToken( tokentype ) ;
      } 
      
      
      else {
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      } 
      
      while ( tokentype == LOR ) {
        
        if ( !mIsCompute ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
        } 
        
        
        else {
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex = nextTokenIndex + 1 ;
        } 
        
        
        if ( Maybe_bit_ex_OR_exp( nextTokenIndex ) ) {
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      return true ;
    } 
    
    return false ; 
  } 
  
  bool Maybe_bit_ex_OR_exp( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
    
    
    if ( Maybe_bit_AND_exp( nextTokenIndex ) ) {
      
      if ( !mIsCompute ) {
        
        PeekToken( tokentype ) ;
      } 
      
      
      else {
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      } 
      
      while ( tokentype == POWER ) {
        
        
        if ( !mIsCompute ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
        } 
        
        
        else {
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex = nextTokenIndex + 1 ;
        } 
        
        
        if ( Maybe_bit_AND_exp( nextTokenIndex ) ) {
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      return true ;
    } 
    
    return false ; 
  } 
  
  bool Rest_of_maybe_bit_ex_OR_exp( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
    
    
    if ( Rest_of_maybe_bit_AND_exp( nextTokenIndex ) ) {
      
      if ( !mIsCompute ) {
        
        PeekToken( tokentype ) ;
      } 
      
      
      else {
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      } 
      
      while ( tokentype == POWER ) {
        
        
        if ( !mIsCompute ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
        } 
        
        
        else {
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex = nextTokenIndex + 1 ;
        } 
        
        
        if ( Maybe_bit_AND_exp( nextTokenIndex ) ) {
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      return true ;
    } 

    return false ; 
  } 
  
  bool Maybe_bit_AND_exp( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
    
    
    if ( Maybe_equality_exp( nextTokenIndex ) ) {
      
      if ( !mIsCompute ) {
        
        PeekToken( tokentype ) ;
      } 
      
      
      else {
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      } 
      
      while ( tokentype == LAND ) {
        
        
        if ( !mIsCompute ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
        } 
        
        
        else {
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex = nextTokenIndex + 1 ;
        } 
        
        
        if ( Maybe_equality_exp( nextTokenIndex ) ) {
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      return true ;
    } 
  
    return false ; 
  } 
  
  bool Rest_of_maybe_bit_AND_exp( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
    
    
    if ( Rest_of_maybe_equality_exp( nextTokenIndex ) ) {
      
      if ( !mIsCompute ) {
        
        PeekToken( tokentype ) ;
      } 
      
      
      else {
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      } 
      
      while ( tokentype == LAND ) {
        
        
        if ( !mIsCompute ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
        } 
        
        
        else {
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex = nextTokenIndex + 1 ;
        } 
        
        
        if ( Maybe_equality_exp( nextTokenIndex ) ) {
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      return true ;
    } 
    
    return false ; 
  } 
  
  bool Maybe_equality_exp( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
    
    
    if ( Maybe_relational_exp( nextTokenIndex ) ) {
      
      if ( !mIsCompute ) {
        
        PeekToken( tokentype ) ;
      } 
      
      
      else {
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      } 
      
      while ( tokentype == EQ || tokentype == NEQ ) {
        
        
        if ( !mIsCompute ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
        } 
        
        
        else {
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex = nextTokenIndex + 1 ;
        } 
        
        
        if ( Maybe_relational_exp( nextTokenIndex ) ) {
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      return true ;
    } 
    
    return false ; 
  } 
  
  bool Rest_of_maybe_equality_exp( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
    
    
    if ( Rest_of_maybe_relational_exp( nextTokenIndex ) ) {
      
      if ( !mIsCompute ) {
        
        PeekToken( tokentype ) ;
      } 
      
      
      else {
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      } 
      
      while ( tokentype == EQ || tokentype == NEQ ) {
        
        
        if ( !mIsCompute ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
        } 
        
        
        else {
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex = nextTokenIndex + 1 ;
        } 
        
        
        if ( Maybe_relational_exp( nextTokenIndex ) ) {
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      return true ;
    } 
    
    return false ; 
  } 
  
  bool Maybe_relational_exp( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
    
    
    if ( Maybe_shift_exp( nextTokenIndex ) ) {
      
      if ( !mIsCompute ) {
        
        PeekToken( tokentype ) ;
      } 
      
      
      else {
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      } 
      
      while ( tokentype == GT || tokentype == ST || tokentype == LE || tokentype == GE ) {
        
        
        if ( !mIsCompute ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
        } 
        
        
        else {
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex = nextTokenIndex + 1 ;
        } 
        
        
        if ( Maybe_shift_exp( nextTokenIndex ) ) {
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      return true ;
    } 
     
    return false ; 
  } 
  
  bool Rest_of_maybe_relational_exp( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
    
    
    if ( Rest_of_maybe_shift_exp( nextTokenIndex ) ) {
      
      if ( !mIsCompute ) {
        
        PeekToken( tokentype ) ;
      } 
      
      
      else {
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      } 
      
      while ( tokentype == GT || tokentype == ST || tokentype == LE || tokentype == GE ) {
        
        
        if ( !mIsCompute ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
        } 
        
        
        else {
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex = nextTokenIndex + 1 ;
        } 
        
        
        if ( Maybe_shift_exp( nextTokenIndex ) ) {
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      return true ;
    } 
    
    return false ;  
  } 
  
  bool Maybe_shift_exp( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
    
    
    if ( Maybe_additive_exp( nextTokenIndex ) ) {
      
      if ( !mIsCompute ) {
        
        PeekToken( tokentype ) ;
      } 
      
      
      else {
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      } 
      
      while ( tokentype == LS || tokentype == RS ) {
        
        
        if ( !mIsCompute ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
        } 
        
        
        else {
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex = nextTokenIndex + 1 ;
        } 
        
        
        if ( Maybe_additive_exp( nextTokenIndex ) ) {
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      return true ;
    } 
    
    return false ; 
  } 
  
  bool Rest_of_maybe_shift_exp( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;

    
    if ( Rest_of_maybe_additive_exp( nextTokenIndex ) ) {
      
      
      if ( !mIsCompute ) {
        
        PeekToken( tokentype ) ;
      } 
      
      
      else {
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      } 
      
      while ( tokentype == LS || tokentype == RS ) {
        
        
        if ( !mIsCompute ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
        } 
        
        
        else {
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex = nextTokenIndex + 1 ;
        } 
        
        
        if ( Maybe_additive_exp( nextTokenIndex ) ) {
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      return true ;
    } 
  
    return false ; 
  } 
  
  bool Maybe_additive_exp( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
    
    
    if ( Maybe_mult_exp( nextTokenIndex ) ) {
      
      if ( !mIsCompute ) {
        
        PeekToken( tokentype ) ;
      } 
      
      
      else {
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      } 
      
      while ( tokentype == PLUS || tokentype == MINUS ) {
        
        
        if ( !mIsCompute ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
        } 
        
        
        else {
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex = nextTokenIndex + 1 ;
        } 
        
        
        if ( Maybe_mult_exp( nextTokenIndex ) ) {
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      return true ;
    } 
    
    return false ; 
  } 
  
  bool Rest_of_maybe_additive_exp( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
    
    
    if ( Rest_of_maybe_mult_exp( nextTokenIndex ) ) {
      
      if ( !mIsCompute ) {
        
        PeekToken( tokentype ) ;
      } 
      
      
      else {
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      } 
      
      while ( tokentype == PLUS || tokentype == MINUS ) {
        
        
        if ( !mIsCompute ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
        } 
        
        
        else {
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex = nextTokenIndex + 1 ;
        } 
        
        
        if ( Maybe_mult_exp( nextTokenIndex ) ) {
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      return true ;
    } 

    return false ; 
  } 
  
  bool Maybe_mult_exp( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
    
    
    if ( Unary_exp( nextTokenIndex ) ) {
      
      if ( Rest_of_maybe_mult_exp( nextTokenIndex ) ) return true ; 
      else return false ; 
    } 
    
    return false ; 
  } 
  
  bool Rest_of_maybe_mult_exp( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
    
    
    if ( !mIsCompute ) {
      
      PeekToken( tokentype ) ;
    } 
    
    
    else {
      
      PassTwoPeekToken( nextTokenIndex, tokentype ) ;
    } 
    
    while ( tokentype == MUL || tokentype == DIV || tokentype == MOD ) {
      
      
      if ( !mIsCompute ) {
        
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
      } 
      
      
      else {
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex = nextTokenIndex + 1 ;
      } 
      
      
      if ( Unary_exp( nextTokenIndex ) ) {
        
        if ( !mIsCompute ) {
          
          PeekToken( tokentype ) ;
        } 
        
        
        else {
          
          PassTwoPeekToken( nextTokenIndex, tokentype ) ;
        } 
      } 
      
      else return false ; 
    } 
    
    return true ;
  } 
  
  bool Unary_exp( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
    int index = 0 ;
    
    
    if ( !mIsCompute ) {
      
      PeekToken( tokentype ) ;
      
      if ( Sign( tokentype ) ) {
        
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        PeekToken( tokentype ) ;
        
        while ( Sign( tokentype ) ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
          
          
          PeekToken( tokentype ) ;
        } 
        
        
        if ( Signed_unary_exp( nextTokenIndex ) ) return true ;
        else return false ; 
      } 
      
      else if ( tokentype == PP || tokentype == MM ) {
        
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        if ( tokentype == IDENTIFIER ) {
          
          
          
          if ( !IsSystemSupportFunction( gToken.tokenID ) ) {
            
            if ( mUserInputType == DEFINEFUNCTION ) {
              
              index = FindTokenInSymbolList( gToken.tokenID, mNewDefineFunc.localSymbolList ) ;
              
              
              if ( index == -1 ) {
                
                index = FindTokenInSymbolList( gToken.tokenID, gGlobalSymbolList ) ;
                
                
                if ( index == -1 ) {
                  
                  index = FindTokenInFunctionList( gToken.tokenID ) ;
                  
                  
                  if ( index == -1 ) {
                    
                    throw new Exception3( gToken.tokenID ) ;
                    return false ;
                  } 
                } 
              } 
            } 
            
            
            else {
              if ( mUserInputType == LOCALSTATEMENT ) {
                
                index = FindTokenInSymbolList( gToken.tokenID, mStmtNewDefineSymbolList ) ;
                
                
                if ( index == -1 ) {
                  
                  
                  index = FindTokenInSymbolList( gToken.tokenID, gGlobalSymbolList ) ;
                  
                  
                  if ( index == -1 ) {
                    
                    index = FindTokenInFunctionList( gToken.tokenID ) ;
                    
                    
                    if ( index == -1 ) {
                      
                      throw new Exception3( gToken.tokenID ) ;
                      return false ;
                    } 
                  } 
                } 
              } 
              
              else {
                
                index = FindTokenInSymbolList( gToken.tokenID, gGlobalSymbolList ) ;
                
                
                if ( index == -1 ) {
                  
                  index = FindTokenInFunctionList( gToken.tokenID ) ;
                  
                  
                  if ( index == -1 ) {
                    
                    throw new Exception3( gToken.tokenID ) ;
                    return false ;
                  } 
                } 
              } 
            } 
          } 
          
          
          
          PeekToken( tokentype ) ;
          
          
          if ( tokentype == LBK ) {
            
            
            GetToken( tokentype ) ;
            gAllTokenStringList.push_back( gToken ) ;
            
            
            if ( Expression( nextTokenIndex ) ) {
              
              GetToken( tokentype ) ;
              gAllTokenStringList.push_back( gToken ) ;
              
              if ( tokentype == RBK ) {
                return true ;
              } 
              
              else {
                
                ThrowException( tokentype ) ;
                return false ;  
              } 
            } 
            
            else return false ; 
          } 
          
          return true ;
        } 
        
        
        else {
          
          ThrowException( tokentype ) ;
          return false ; 
        } 
      } 
      
      
      else if ( tokentype == IDENTIFIER || tokentype == CONSTANT_INT || tokentype == CONSTANT_FLOAT 
                || tokentype == CONSTANT_CHAR
                || tokentype == CONSTANT_STRING || tokentype == CONSTANT_BOOL || tokentype == LP ) {
                  
        
        if ( Unsigned_unary_exp( nextTokenIndex ) ) return true ;
        else return false ; 
      } 
      
      
      else {
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        ThrowException( tokentype ) ;
        return false ;
      } 
    } 
    
    
    else {
      
    } 
    
    return false ;
  } 
  
  bool Signed_unary_exp( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
    int index = 0 ;
    
    
    if ( !mIsCompute ) {
      
      GetToken( tokentype ) ;
      gAllTokenStringList.push_back( gToken ) ;
      
      if ( tokentype == IDENTIFIER ) {
        
        
        
        if ( !IsSystemSupportFunction( gToken.tokenID ) ) {
          
          if ( mUserInputType == DEFINEFUNCTION ) {
            
            index = FindTokenInSymbolList( gToken.tokenID, mNewDefineFunc.localSymbolList ) ;
            
            
            if ( index == -1 ) {
              
              index = FindTokenInSymbolList( gToken.tokenID, gGlobalSymbolList ) ;
              
              
              if ( index == -1 ) {
                
                index = FindTokenInFunctionList( gToken.tokenID ) ;
                
                
                if ( index == -1 ) {
                  
                  throw new Exception3( gToken.tokenID ) ;
                  return false ;
                } 
              } 
            } 
          } 
          
          
          else {
            if ( mUserInputType == LOCALSTATEMENT ) {
              
              index = FindTokenInSymbolList( gToken.tokenID, mStmtNewDefineSymbolList ) ;
              
              
              if ( index == -1 ) {
                
                
                index = FindTokenInSymbolList( gToken.tokenID, gGlobalSymbolList ) ;
                
                
                if ( index == -1 ) {
                  
                  index = FindTokenInFunctionList( gToken.tokenID ) ;
                  
                  
                  if ( index == -1 ) {
                    
                    throw new Exception3( gToken.tokenID ) ;
                    return false ;
                  } 
                } 
              } 
            } 
            
            else {
              
              index = FindTokenInSymbolList( gToken.tokenID, gGlobalSymbolList ) ;
              
              
              if ( index == -1 ) {
                
                index = FindTokenInFunctionList( gToken.tokenID ) ;
                
                
                if ( index == -1 ) {
                  
                  throw new Exception3( gToken.tokenID ) ;
                  return false ;
                } 
              } 
            } 
          } 
        } 
        
         
        
        PeekToken( tokentype ) ;
        
        
        if ( tokentype == LP || tokentype == LBK ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
          
          
          if ( tokentype == LP ) {
            
            
            PeekToken( tokentype ) ;
            
            
            if ( tokentype == IDENTIFIER || tokentype == PP || tokentype == MM 
                 || Sign( tokentype ) || tokentype == CONSTANT_INT 
                 || tokentype == CONSTANT_FLOAT || tokentype == CONSTANT_CHAR 
                 || tokentype == CONSTANT_STRING || tokentype == CONSTANT_BOOL
                 || tokentype == LP ) {
              
              
              if ( !Actual_parameter_list( nextTokenIndex ) ) {
                return false ;
              } 
            } 
            
            
            GetToken( tokentype ) ;
            gAllTokenStringList.push_back( gToken ) ;
            
            
            if ( tokentype == RP ) {
              return true ;
            } 
            
            
            else {
              
              ThrowException( tokentype ) ;
              return false ;
            } 
          } 
          
          
          else {
            
            if ( Expression( nextTokenIndex ) ) {
              
              GetToken( tokentype ) ;
              gAllTokenStringList.push_back( gToken ) ;
              
              
              if ( tokentype == RBK ) {
                return true ;
              } 
              
              
              else {
                
                ThrowException( tokentype ) ;
                return false ;
              } 
            } 
            
            else return false ; 
          } 
        } 
        
        return true ;
      } 
      
      else if ( tokentype == CONSTANT_INT || tokentype == CONSTANT_FLOAT || 
                tokentype == CONSTANT_CHAR || tokentype == CONSTANT_STRING || tokentype == CONSTANT_BOOL ) {
        return true ;
      } 
      
      else if ( tokentype == LP ) {
        
        if ( Expression( nextTokenIndex ) ) {
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
          
          
          if ( tokentype == RP ) {
            return true ;
          } 
          
          
          else {
            
            ThrowException( tokentype ) ;
            return false ;
          } 
        } 
        
        else return false ; 
      } 
      
      
      else {
        
        ThrowException( tokentype ) ;
        return false ;
      } 
    } 
    
    
    else {
      
    } 
    
    return false ;
  } 
  
  bool Unsigned_unary_exp( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
    int index = 0 ;
    
    
    if ( !mIsCompute ) {
      
      GetToken( tokentype ) ;
      gAllTokenStringList.push_back( gToken ) ;
      
      if ( tokentype == IDENTIFIER ) {
        
        
        
        if ( !IsSystemSupportFunction( gToken.tokenID ) ) {
          
          if ( mUserInputType == DEFINEFUNCTION ) {
            
            index = FindTokenInSymbolList( gToken.tokenID, mNewDefineFunc.localSymbolList ) ;
            
            
            if ( index == -1 ) {
              
              index = FindTokenInSymbolList( gToken.tokenID, gGlobalSymbolList ) ;
              
              
              if ( index == -1 ) {
                
                index = FindTokenInFunctionList( gToken.tokenID ) ;
                
                
                if ( index == -1 ) {
                  
                  throw new Exception3( gToken.tokenID ) ;
                  return false ;
                } 
              } 
            } 
          } 
          
          
          else {
            if (  mUserInputType == LOCALSTATEMENT ) {
              
              index = FindTokenInSymbolList( gToken.tokenID, mStmtNewDefineSymbolList ) ;
              
              
              if ( index == -1 ) {
                
                
                index = FindTokenInSymbolList( gToken.tokenID, gGlobalSymbolList ) ;
                
                
                if ( index == -1 ) {
                  
                  index = FindTokenInFunctionList( gToken.tokenID ) ;
                  
                  
                  if ( index == -1 ) {
                    
                    throw new Exception3( gToken.tokenID ) ;
                    return false ;
                  } 
                } 
              } 
            } 
            
            else {
              
              index = FindTokenInSymbolList( gToken.tokenID, gGlobalSymbolList ) ;
              
              
              if ( index == -1 ) {
                
                index = FindTokenInFunctionList( gToken.tokenID ) ;
                
                
                if ( index == -1 ) {
                  
                  throw new Exception3( gToken.tokenID ) ;
                  return false ;
                } 
              } 
            } 
          } 
        } 
        
         
        
        PeekToken( tokentype ) ;
        
        
        if ( tokentype == LP || tokentype == LBK || tokentype == PP || tokentype == MM ) {
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
          
          if ( tokentype == LP ) {
            
            PeekToken( tokentype ) ;
            
            
            if ( tokentype == IDENTIFIER || tokentype == PP || tokentype == MM 
                 || Sign( tokentype ) || tokentype == CONSTANT_INT 
                 || tokentype == CONSTANT_FLOAT || tokentype == CONSTANT_CHAR 
                 || tokentype == CONSTANT_STRING || tokentype == CONSTANT_BOOL
                 || tokentype == LP ) {
              
              
              if ( !Actual_parameter_list( nextTokenIndex ) ) {
                return false ;
              } 
            } 
            
            
            GetToken( tokentype ) ;
            gAllTokenStringList.push_back( gToken ) ;
            
            
            if ( tokentype == RP ) {
              return true ;
            } 
            
            
            else {
              
              ThrowException( tokentype ) ;
              return false ;
            } 
          } 
          
          else if ( tokentype == LBK ) {
            
            
            if ( Expression( nextTokenIndex ) ) {
              
              GetToken( tokentype ) ;
              gAllTokenStringList.push_back( gToken ) ;
              
              
              if ( tokentype == RBK ) {
                
                PeekToken( tokentype ) ;
                
                
                if ( tokentype == PP || tokentype == MM ) {
                  
                  GetToken( tokentype ) ;
                  gAllTokenStringList.push_back( gToken ) ;
                  
                  return true ;
                } 
                
                return true ;
              } 
              
              
              else {
                
                ThrowException( tokentype ) ;
                return false ;
              } 
            } 
            
            else return false ; 
          } 
          
          
          else {
            return true ;
          } 
        } 
        
        return true;
      } 
      
      else if ( tokentype == CONSTANT_INT || tokentype == CONSTANT_FLOAT || 
                tokentype == CONSTANT_CHAR || tokentype == CONSTANT_STRING || tokentype == CONSTANT_BOOL ) {
        return true ;
      } 
      
      else if ( tokentype == LP ) {
        
        if ( Expression( nextTokenIndex ) ) {
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
          
          
          if ( tokentype == RP ) {
            return true ;
          } 
          
          
          else {
            
            ThrowException( tokentype ) ;
            return false ;
          } 
        } 
        
        else return false ; 
      } 
      
      
      else {
        
        ThrowException( tokentype ) ;
        return false ;
      } 
    } 
    
    
    else {
      
    } 
    
    return false ;
  } 
   
  
  void ThrowException( TOKENTYPE tokentype ) {
    
    if ( tokentype == UNRECOGNIZE_TOKEN ) {
      throw new Exception1( gToken.tokenID ) ;
    } 

    
    else if ( tokentype == TOKEN_IS_EOF ) {
      throw new Exception4( "Error" ) ;
    } 

    
    else {
      throw new Exception2( gToken.tokenID ) ;
    } 
  } 
  
  
  void SkipErrorLine() {
    while ( gLine == gAllTokenStringList[gAllTokenStringList.size()-1].tokenLine ) {
      Get_next_ch( gnextchar ) ;
    } 
  } 
  
  void SortFunctionName() {
    for ( int i = 0 ; i < gGlobalFuncSymbolList.size()-1 ; i++ ) {
      for ( int j = 0 ; j < gGlobalFuncSymbolList.size()-i-1 ; j++ ) {
        if ( gGlobalFuncSymbolList[j].name.compare( gGlobalFuncSymbolList[j+1].name ) > 0 ) {
          FunctionInfo temp = gGlobalFuncSymbolList[j] ;
          gGlobalFuncSymbolList[j] = gGlobalFuncSymbolList[j+1] ;
          gGlobalFuncSymbolList[j+1] = temp ;
        } 
      } 
    } 
  } 
  
  
  void SortSymbolName() {
    for ( int i = 0 ; i < gGlobalSymbolList.size()-1 ; i++ ) {
      for ( int j = 0 ; j < gGlobalSymbolList.size()-i-1 ; j++ ) {
        if ( gGlobalSymbolList[j].name.compare( gGlobalSymbolList[j+1].name ) > 0 ) {
          IndentifierSymbolInfo temp = gGlobalSymbolList[j] ;
          gGlobalSymbolList[j] = gGlobalSymbolList[j+1] ;
          gGlobalSymbolList[j+1] = temp ;
        } 
      } 
    } 
  } 
  
  void PrintGlobalSymbolList() {
    for ( int i = 0 ; i < gGlobalSymbolList.size() ; i++ ) {
      cout << "Name: " << gGlobalSymbolList[i].name << " , ( " ;
      cout << gGlobalSymbolList[i].symbolStartLine << " , " ;
      cout << gGlobalSymbolList[i].symbolStartColumn << " ) " << endl ;
    } 
  } 
  
  void PrintGlobalFunctionList() {
    for ( int i = 0 ; i < gGlobalFuncSymbolList.size() ; i++ ) {
      cout << "Name: " << gGlobalFuncSymbolList[i].name << " , ( " ; 
      cout << gGlobalFuncSymbolList[i].funcStartLine << " , "  ;
      cout << gGlobalFuncSymbolList[i].funcStartColumn << " ) " << endl ;
    } 
  } 
  
  void PrintAllTokenInTokenStringList() {
    for ( int i = 0 ; i < gAllTokenStringList.size() ; i++ ) {
      cout << gAllTokenStringList[i].tokenID << "  ( " << gAllTokenStringList[i].tokenLine ;
      cout <<  " , " << gAllTokenStringList[i].tokenColumn << " )" << endl ;
    } 
  } 
  
  void PrintListAllVariables() {
    if ( !gGlobalSymbolList.empty() ) {
      SortSymbolName() ;
      for ( int i = 0 ; i < gGlobalSymbolList.size() ; i++ ) {
        cout << gGlobalSymbolList[i].name << endl ;
      } 
    } 
  } 
  
  void PrintListAllFunctions() {
    if ( !gGlobalFuncSymbolList.empty() ) {
      SortFunctionName() ;
      for ( int i = 0 ; i < gGlobalFuncSymbolList.size() ; i++ ) {
        cout << gGlobalFuncSymbolList[i].name << "()" << endl ;
      } 
    } 
  } 
  
  void PrintListVariable( string str ) {
    int space = 0 ;
    int index = 0 ;
    int startIndex = 0 ;
    
    
    index = FindTokenInSymbolList( str, gGlobalSymbolList ) ;
    
    if ( index != -1 ) {
      
      
      startIndex = FindTokenInAllTokenStringList( gGlobalSymbolList[index].symbolStartLine, 
                                                  gGlobalSymbolList[index].symbolStartColumn ) ;
      
      
      if ( gGlobalSymbolList[index].symbolReturnType == VOID ) cout << "void "  ;
      else if ( gGlobalSymbolList[index].symbolReturnType == INT ) cout << "int "  ;
      else if ( gGlobalSymbolList[index].symbolReturnType == FLOAT ) cout << "float "  ;
      else if ( gGlobalSymbolList[index].symbolReturnType == CHAR ) cout << "char "  ;
      else if ( gGlobalSymbolList[index].symbolReturnType == STRING ) cout << "string "  ;
      else if ( gGlobalSymbolList[index].symbolReturnType == BOOL ) cout << "bool "  ;
      
      
      
      while ( gAllTokenStringList[startIndex].tokentype != SEMICOLON &&
              gAllTokenStringList[startIndex].tokentype != COMMA ) {
        
        if ( gAllTokenStringList[startIndex].tokentype == LBK ) {
          cout << gAllTokenStringList[startIndex].tokenID << " " ;
          startIndex++ ;
          cout << gAllTokenStringList[startIndex].tokenID << " " ;
          startIndex++ ;
        } 
        
        else {
          cout << gAllTokenStringList[startIndex].tokenID ;
          startIndex++ ;
        } 
      } 
      
      
      cout << " ;" << endl ;
    } 
    
  } 
  
  void PrintListFunction( string str ) {
    int space = 0 ;
    int index = 0 ;
    int startIndex = 0 ;
    int endIndex = 0 ;
    
    
    index = FindTokenInFunctionList( str ) ;
    
    if ( index != -1 ) {
      
      
      startIndex = FindTokenInAllTokenStringList( gGlobalFuncSymbolList[index].funcStartLine, 
                                                  gGlobalFuncSymbolList[index].funcStartColumn ) ;
      
      
      endIndex = FindTokenInAllTokenStringList( gGlobalFuncSymbolList[index].funcEndLine, 
                                                gGlobalFuncSymbolList[index].funcEndColumn ) ;
      
      
      while ( startIndex != endIndex+1 ) {
        
        if ( gAllTokenStringList[startIndex].tokentype == RBC ) space = space - 2 ;
        
        
        for ( int i = 0 ; i < space ; i++ ) 
          cout << " " ;
        
        cout << gAllTokenStringList[startIndex].tokenID ;
        
        if ( gAllTokenStringList[startIndex].tokentype == RBC ) {
          cout << endl ;
        } 
        
        startIndex++ ;
        
        if ( startIndex > endIndex  ) return ;
        
        if ( gAllTokenStringList[startIndex-1].tokentype != RBC ) {
          
          
          while ( gAllTokenStringList[startIndex].tokentype != LBC
                  && gAllTokenStringList[startIndex].tokentype != SEMICOLON 
                  && gAllTokenStringList[startIndex].tokentype != RBC ) {
            
            if ( gAllTokenStringList[startIndex].tokentype == PP 
                 || gAllTokenStringList[startIndex].tokentype == MM ) {
              if ( gAllTokenStringList[startIndex-1].tokentype == IDENTIFIER )
                cout << gAllTokenStringList[startIndex].tokenID ;
              else
                cout << " " << gAllTokenStringList[startIndex].tokenID ;
              startIndex ++ ;
            } 
            
            else if ( gAllTokenStringList[startIndex].tokentype == COMMA ) {
              cout << gAllTokenStringList[startIndex].tokenID ;
              startIndex ++ ;
            } 
            
            else if ( gAllTokenStringList[startIndex].tokentype == LP ) {
              if ( gAllTokenStringList[startIndex-1].tokentype == IDENTIFIER && 
                   ( gAllTokenStringList[startIndex-1].tokentype != IF 
                     && gAllTokenStringList[startIndex-1].tokentype != WHILE 
                     && gAllTokenStringList[startIndex-1].tokentype != ELSE ) ) {
                cout << gAllTokenStringList[startIndex].tokenID ;
              } 
              
              else {
                cout << " " << gAllTokenStringList[startIndex].tokenID ;
              } 
              
              startIndex ++ ;
            } 
            
            
            else if ( gAllTokenStringList[startIndex].tokentype == LBK ) {
              cout << gAllTokenStringList[startIndex].tokenID ;
              startIndex ++ ;
            } 
            
            else if ( gAllTokenStringList[startIndex].tokentype == RP ) {
              if ( gAllTokenStringList[startIndex-1].tokentype == LP ) 
                cout << gAllTokenStringList[startIndex].tokenID ;
              else cout << " " << gAllTokenStringList[startIndex].tokenID ;
              startIndex ++ ;
            } 
            
            
            else {
              cout << " " << gAllTokenStringList[startIndex].tokenID ;
              startIndex ++ ;
            } 
          } 
          
          
          if ( gAllTokenStringList[startIndex].tokentype == LBC ) {

            cout << " " << gAllTokenStringList[startIndex].tokenID << endl ;
            startIndex ++ ;
            space = space + 2 ; 
            
          } 
          
          
          else if ( gAllTokenStringList[startIndex].tokentype == SEMICOLON ) {
            
            
            cout << " " << gAllTokenStringList[startIndex].tokenID << endl ;
            startIndex ++ ;
          } 
  
          if ( startIndex == endIndex + 1 ) return ;
        } 
      } 
    } 
    
  } 
} ;


int main() {
  SYNTAX s ;
  bool isQuit = false ;
  int userInputFirstTokenLine, userIntpuFirstTOkenColumn = 0 ;
  int pointToTheLastTokenLine = 1 ;
  int line = 0 ;
  int uTestNum = 0 ;
  
  s.TokenInit( gToken ) ;
  s.TokenInit( gPeekToken ) ;
  
  cin >> uTestNum ;
  cout << "Our-C running ..." << endl ;
  
  while ( !isQuit ) {
    try {
      
      cout << "> " ;
      
      
      if ( s.A_user_input( userInputFirstTokenLine, userIntpuFirstTOkenColumn, isQuit ) ) {
        /*
        cout << "Global symbol: " << endl ;
        s.PrintGlobalSymbolList() ;
        cout << "Global Function: " << endl ;
        s.PrintGlobalFunctionList() ;
        cout << "All Token List: " << endl ;
        s.PrintAllTokenInTokenStringList() ;
        */
        
        
        if ( isQuit ) {
          cout << "Our-C exited ..." ;
          return 0 ;
        } 
      } 
    } 
    
    catch ( Exception1 * exp1 ) { 
      line = gAllTokenStringList[gAllTokenStringList.size()-1].tokenLine - pointToTheLastTokenLine ;
      if ( line == 0  ) line = 1 ;
      cout << "Line " << line << " : " << "unrecognized token with first char : '" ;
      cout <<  exp1->message << "'" << endl;
      s.SkipErrorLine() ;
    } 
    catch ( Exception2 * exp2 ) { 
      line = gAllTokenStringList[gAllTokenStringList.size()-1].tokenLine - pointToTheLastTokenLine ;
      if ( line == 0  ) line = 1 ;
      cout << "Line " << line << " : " << "unexpected token : '" << exp2->message << "'" << endl;
      s.SkipErrorLine() ;
    } 
    catch ( Exception3 * exp3 ) {
      
      
      line = gAllTokenStringList[gAllTokenStringList.size()-1].tokenLine  - pointToTheLastTokenLine ;
      if ( line == 0  ) line = 1 ;
      cout << "Line " << line << " : "  << "undefined identifier : '" << exp3->message << "'" << endl;
      s.SkipErrorLine() ;
    } 
    catch ( Exception4 * exp4 ) {
      cout << exp4->message << endl;
      s.SkipErrorLine() ;
    } 
    
    
    pointToTheLastTokenLine = gAllTokenStringList[gAllTokenStringList.size()-1].tokenLine ; 
    s.FunctionInit( s.mNewDefineFunc ) ; 
    s.mNewDefineSymbolList.clear() ; 
    s.mStmtNewDefineSymbolList.clear() ; 
    if ( gPeekToken.tokenID.empty() && gisEOF ) isQuit = true ;
  } 
  
  
  return 0;
} 
