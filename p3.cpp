# include <iostream>
# include <string>
# include <string.h>
# include <vector>
# include <cstdlib>
# include <stdio.h>
# include <iomanip>
# include <sstream>

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
  UNRECOGNIZE_TOKEN, TRUE 
};

enum USERINPUTTYPE {
  DEFINESYMBOL, DEFINEFUNCTION, STATEMENT
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

struct ArrayInfo{
  int arrayTable ;    
  int arrayEntry ;
  int size ;     
};

struct RESULT{
  TOKENTYPE valueType ;
  double numValue ;
  char charValue ;
  bool boolValue ;
  string stringValue ;
  string idName ;
};

struct RETURNID {
  bool changeValue ;
  bool isArray ;
  bool isIdGlobal ;
  int idIndex ;
  int arrayIndex ;
  TOKENTYPE returnOperationValue ;
} ;


vector<double> gArrayIntTable ;  
vector<double> gArrayFloatTable ; 
vector<char> gArrayCharTable ; 
vector<bool> gArrayBoolTable ; 
vector<string> gArrayStringTable ; 
vector<ArrayInfo> gArrayInfoTable ; 

struct ArgsDefineInfo{
  string name ;
  TOKENTYPE argType ;         
  TOKENTYPE argReturnType ;  
  int arrayInfoEntry ;       
  bool isReturn ;            
} ;

struct IndentifierSymbolInfo{
  string name ;                 
  TOKENTYPE symbolType ;        
  TOKENTYPE symbolReturnType ;  
  int symbolStartLine ;         
  int symbolStartColumn ;       
  int arrayEntry ;              
  RESULT symbolValue ;
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

struct LOCAL {
  vector<IndentifierSymbolInfo> localList ;  
};


vector<IndentifierSymbolInfo> gGlobalSymbolList ;      
vector<FunctionInfo> gGlobalFuncSymbolList ;           
vector<LOCAL> gCallStack ;                             
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
  USERINPUTTYPE mUserInputType ; 
  int mCoutExpFalse ;
  bool mDoExp ;
  
  
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
    newDefineSymbol.arrayEntry = -1 ;
    newDefineSymbol.symbolValue.numValue = 0 ;
    newDefineSymbol.symbolValue.charValue = '\0' ;
    newDefineSymbol.symbolValue.stringValue = "" ;
    newDefineSymbol.symbolValue.boolValue = false ;
  } 
  
  
  void TokenInit( TOKEN &token ) {
    token.tokenID = "" ;
    token.tokenLine = 0 ;
    token.tokenColumn = 0 ;
    token.tokentype = UNRECOGNIZE_TOKEN ;
    
  } 
  
  int StringToInt( string str ) {
    return atoi( str.c_str() ) ;
  } 
  
  double StringToFloat( string str ) {
    return atof( str.c_str() ) ;
  } 
  
  string IntToString( int num ) {
    stringstream ss ;
    ss << num ;
    return ss.str() ;
  } 

  string FloatToString( float num ) {
    stringstream ss ;
    ss << num ;
    return ss.str() ;
  } 
  
  string DoubleToString( double num ) {
    stringstream ss ;
    ss << num ;
    return ss.str() ;
  } 

  string CopyString( string token ) {
    string str = "" ;
    for ( int i = 0 ; i < token.size() ; i++ ) {
      str = str + token[i] ;
    } 
  
    return str ;
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
      
      gGlobalFuncSymbolList.push_back( mNewDefineFunc ) ;
      
      cout << "Definition of " <<  mNewDefineFunc.name << "() entered ..." << endl ;
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
      
      gGlobalSymbolList.push_back( symbolInfo ) ;
      
      cout << "Definition of " <<  symbolInfo.name << " entered ..." << endl ;
    } 
  } 
  
  
  
  void InsertSymbolToLocalSymbolList( IndentifierSymbolInfo symbolInfo, int startIndex ) {
    int index = -1 ;
    int c = 0 ;
    int callStackLastIndex = -1 ;
    int localVarLastIndex = -1 ;
    
    
    callStackLastIndex = gCallStack.size()-1 ;
    c = callStackLastIndex ;
    localVarLastIndex = gCallStack[callStackLastIndex].localList.size()-1 ;
    
    
    for ( int i = startIndex ; i <= localVarLastIndex ; i++ ) {
      
      if ( gCallStack[callStackLastIndex].localList[i].name.compare( symbolInfo.name ) == 0 ) {
        index = i ;  
        i = localVarLastIndex + 1 ; 
      } 
    } 
    
    
    if ( index != -1 ) {
      
      
      gCallStack[c].localList.erase( 
                                     gCallStack[c].localList.begin() + index ) ;
          
      
      gCallStack[callStackLastIndex].localList.push_back( symbolInfo ) ;
    } 
    
    
    else {
      
      gCallStack[callStackLastIndex].localList.push_back( symbolInfo ) ;
    } 
  } 
  
  
  
  
  void SetArrayInfo( IndentifierSymbolInfo &newDefineSymbol, TOKEN sizeInfo ) {
    ArrayInfo arrayInfo ;
    
    if ( sizeInfo.tokentype == CONSTANT_INT ) {
      
      arrayInfo.size = StringToInt( sizeInfo.tokenID ) ;
       
      
      
      if ( newDefineSymbol.symbolReturnType == INT ) {
        
        arrayInfo.arrayTable = 1 ;
        
        
        
        
        newDefineSymbol.arrayEntry = gArrayInfoTable.size() ;
        
        
        gArrayInfoTable.push_back( arrayInfo ) ;
      } 
      
      
      else if ( newDefineSymbol.symbolReturnType == FLOAT ) {
        
        arrayInfo.arrayTable = 2 ;
        
        

        
        newDefineSymbol.arrayEntry = gArrayInfoTable.size() ;
        
        
        gArrayInfoTable.push_back( arrayInfo ) ;
        
      } 
      
      
      else if ( newDefineSymbol.symbolReturnType == CHAR ) {
        
        arrayInfo.arrayTable = 3 ;
        
        
        
        
        newDefineSymbol.arrayEntry = gArrayInfoTable.size() ;
        
        
        gArrayInfoTable.push_back( arrayInfo ) ;
      } 
      
      
      else if ( newDefineSymbol.symbolReturnType == STRING ) {
        
        arrayInfo.arrayTable = 5 ;
        
        
        
        
        newDefineSymbol.arrayEntry = gArrayInfoTable.size() ;
        
        
        gArrayInfoTable.push_back( arrayInfo ) ;
      } 
      
      
      else if ( newDefineSymbol.symbolReturnType == BOOL ) {
        
        arrayInfo.arrayTable = 4 ;
        
        
        
        
        newDefineSymbol.arrayEntry = gArrayInfoTable.size() ;
        
        
        gArrayInfoTable.push_back( arrayInfo ) ;
      } 
    } 
  } 
  
  void SetArgArrayInfo( ArgsDefineInfo &newDefineArg, TOKEN sizeInfo ) {
    ArrayInfo arrayInfo ;
    
    if ( sizeInfo.tokentype == CONSTANT_INT ) {
      
      arrayInfo.size = StringToInt( sizeInfo.tokenID ) ;
       
      
      
      if ( newDefineArg.argReturnType == INT ) {
        arrayInfo.arrayTable = 1 ;
        newDefineArg.arrayInfoEntry = gArrayInfoTable.size() ;
        gArrayInfoTable.push_back( arrayInfo ) ;
      } 
      
      
      else if ( newDefineArg.argReturnType == FLOAT ) {
        arrayInfo.arrayTable = 2 ;
        newDefineArg.arrayInfoEntry = gArrayInfoTable.size() ;
        gArrayInfoTable.push_back( arrayInfo ) ;
      } 
      
      
      else if ( newDefineArg.argReturnType == CHAR ) {
        arrayInfo.arrayTable = 3 ;
        newDefineArg.arrayInfoEntry = gArrayInfoTable.size() ;
        gArrayInfoTable.push_back( arrayInfo ) ;
      } 
      
      
      else if ( newDefineArg.argReturnType == STRING ) {
        arrayInfo.arrayTable = 5 ;
        newDefineArg.arrayInfoEntry = gArrayInfoTable.size() ;
        gArrayInfoTable.push_back( arrayInfo ) ;
      } 
      
      
      else if ( newDefineArg.argReturnType == BOOL ) {
        arrayInfo.arrayTable = 4 ;
        newDefineArg.arrayInfoEntry = gArrayInfoTable.size() ;
        gArrayInfoTable.push_back( arrayInfo ) ;
      } 
    } 
  } 
  
  
  
  void SetArraySpace( IndentifierSymbolInfo &newDefineSymbol ) {
    int arrayEntryInfo = -1 ;
    int arraySize = -1 ;
    
    
    arrayEntryInfo =  newDefineSymbol.arrayEntry ;  
    
    
    
    if ( newDefineSymbol.symbolReturnType == INT ) {
      
      
      
      
      gArrayInfoTable[arrayEntryInfo].arrayEntry = gArrayIntTable.size() ;
      
      
      arraySize = gArrayInfoTable[arrayEntryInfo].size ;
      
      
      if ( arraySize != 0 ) {
        for ( int i = 0 ; i < arraySize ; i++ ) {
          gArrayIntTable.push_back( 0 ) ;
        } 
      } 
    } 
    
    
    else if ( newDefineSymbol.symbolReturnType == FLOAT ) {
      
      gArrayInfoTable[arrayEntryInfo].arrayEntry = gArrayFloatTable.size() ;
      
      
      arraySize = gArrayInfoTable[arrayEntryInfo].size ;
      
      
      if ( arraySize != 0 ) {
        for ( int i = 0 ; i < arraySize ; i++ ) {
          gArrayFloatTable.push_back( 0.000 ) ;
        } 
      } 
    } 
    
    
    else if ( newDefineSymbol.symbolReturnType == CHAR ) {
      
      gArrayInfoTable[arrayEntryInfo].arrayEntry = gArrayCharTable.size() ;
      
      
      arraySize = gArrayInfoTable[arrayEntryInfo].size ;
      
      
      if ( arraySize != 0 ) {
        for ( int i = 0 ; i < arraySize ; i++ ) {
          gArrayCharTable.push_back( '\0' ) ;
        } 
      } 
    } 
    
    
    else if ( newDefineSymbol.symbolReturnType == STRING ) {
      
      gArrayInfoTable[arrayEntryInfo].arrayEntry = gArrayStringTable.size() ;
      
      
      arraySize = gArrayInfoTable[arrayEntryInfo].size ;
      
      
      if ( arraySize != 0 ) {
        for ( int i = 0 ; i < arraySize ; i++ ) {
          gArrayStringTable.push_back( "" ) ;
        } 
      } 
    } 
    
    
    else if ( newDefineSymbol.symbolReturnType == BOOL ) {
       
      gArrayInfoTable[arrayEntryInfo].arrayEntry = gArrayBoolTable.size() ;
      
      
      arraySize = gArrayInfoTable[arrayEntryInfo].size ;
      
      
      if ( arraySize != 0 ) {
        for ( int i = 0 ; i < arraySize ; i++ ) {
          gArrayBoolTable.push_back( 0 ) ;
        } 
      } 
    } 

  } 
  
  
  
  void ReturnNonFunctionIdLocation( bool &isIdGlobal, int &idIndex, string id ) {
    
    int callStackLastIndex, localLastIndex = 0 ;
    
    
    idIndex = -1 ;
    isIdGlobal = false ;
    
    
    callStackLastIndex = gCallStack.size()-1 ;
    
    
    
    if ( !gCallStack[callStackLastIndex].localList.empty() ) {
      
      
      localLastIndex = gCallStack[callStackLastIndex].localList.size()-1 ;
      
      
      for ( int i = localLastIndex ; i >= 0 ; i-- ) {
        
        if ( id.compare( gCallStack[callStackLastIndex].localList[i].name ) == 0 ) {
          idIndex = i ;  
          i = -1 ; 
        } 
      } 
      
    } 
    
    
    if ( idIndex == -1 ) {
      
      isIdGlobal = true ;
        
      
      idIndex = FindTokenInSymbolList( id, gGlobalSymbolList ) ;
    } 
     
  } 
  
  
  
  void ReturnIdArrayValue( RESULT &returnResult, bool isIdGlobal, int idIndex, int arrayIndex ) {
    ArrayInfo a ; 
    int callStackLastIndex = 0; 
    int arrayInfoEntry = 0 ; 
    
    
     
    
    if ( isIdGlobal ) {
      arrayInfoEntry = gGlobalSymbolList[idIndex].arrayEntry ;
      a.arrayTable = gArrayInfoTable[arrayInfoEntry].arrayTable ;
      a.arrayEntry = gArrayInfoTable[arrayInfoEntry].arrayEntry ;
  
    } 
    
    
    else {
      callStackLastIndex = gCallStack.size()-1;
      arrayInfoEntry = gCallStack[callStackLastIndex].localList[idIndex].arrayEntry ;
      a.arrayTable = gArrayInfoTable[arrayInfoEntry].arrayTable ;
      a.arrayEntry = gArrayInfoTable[arrayInfoEntry].arrayEntry ;
    } 
    
    
    
    if ( a.arrayTable == 1 ) {
      returnResult.valueType = INT ;
      returnResult.numValue = gArrayIntTable[a.arrayEntry+arrayIndex] ;
    } 
    
    
    else if ( a.arrayTable == 2 ) {
      returnResult.valueType = FLOAT ;
      returnResult.numValue = gArrayFloatTable[a.arrayEntry+arrayIndex] ;
    } 
    
    
    else if ( a.arrayTable == 3 ) {
      returnResult.valueType = CHAR ;
      returnResult.charValue = gArrayCharTable[a.arrayEntry+arrayIndex] ;
    } 
    
    
    else if ( a.arrayTable == 4 ) {
      returnResult.valueType = BOOL ;
      returnResult.boolValue = gArrayBoolTable[a.arrayEntry+arrayIndex] ;
    } 
    
    
    else if ( a.arrayTable == 5 ) {
      returnResult.valueType = STRING ;
      returnResult.stringValue = gArrayStringTable[a.arrayEntry+arrayIndex] ;
    } 
  } 
  
  
  
  void ReturnIdValue( RESULT &returnResult, bool isIdGlobal, int idIndex ) {
    int callStackLastIndex = 0 ; 
    TOKENTYPE idValueType ;  
    
    
    if ( isIdGlobal ) {
      idValueType = gGlobalSymbolList[idIndex].symbolReturnType ;
      returnResult.valueType = idValueType ; 
      
      
      
      if ( idValueType == INT || idValueType == FLOAT ) {
        returnResult.numValue = gGlobalSymbolList[idIndex].symbolValue.numValue ;
      } 

      
      else if ( idValueType == CHAR ) {
        returnResult.charValue = gGlobalSymbolList[idIndex].symbolValue.charValue ;
      } 
      
      
      else if ( idValueType == BOOL ) {
        returnResult.boolValue = gGlobalSymbolList[idIndex].symbolValue.boolValue ;
      } 
      
      
      else if ( idValueType == STRING ) {
        returnResult.stringValue = gGlobalSymbolList[idIndex].symbolValue.stringValue ;
      } 
    } 
    
    
    else {
      callStackLastIndex = gCallStack.size()-1 ;
      idValueType = gCallStack[callStackLastIndex].localList[idIndex].symbolReturnType ;
      
      returnResult.valueType = idValueType ; 
      
      
      
      if ( idValueType == INT || idValueType == FLOAT ) {
        returnResult.numValue = gCallStack[callStackLastIndex].localList[idIndex].symbolValue.numValue ;
      } 

      
      else if ( idValueType == CHAR ) {
        returnResult.charValue = gCallStack[callStackLastIndex].localList[idIndex].symbolValue.charValue ;
      } 
      
      
      else if ( idValueType == BOOL ) {
        returnResult.boolValue = gCallStack[callStackLastIndex].localList[idIndex].symbolValue.boolValue ;
      } 
      
      
      else if ( idValueType == STRING ) {
        returnResult.stringValue = 
        gCallStack[callStackLastIndex].localList[idIndex].symbolValue.stringValue ;
      } 
    } 
  } 
  
  
  void ReturnConstValue( RESULT &returnResult, TOKEN constToken ) {
    char n = '\n' ;
    char t = '\t' ;
    
    
    if ( constToken.tokentype == CONSTANT_INT ) {
      returnResult.valueType = INT ;
      returnResult.numValue = StringToInt( constToken.tokenID ) ;
    } 
    
    
    else if ( constToken.tokentype == CONSTANT_FLOAT ) {
      returnResult.valueType = FLOAT ;
      returnResult.numValue = atof( constToken.tokenID.c_str() ) ;
    } 
    
    
    else if ( constToken.tokentype == CONSTANT_CHAR ) {
      returnResult.valueType = CHAR ;
      if ( constToken.tokenID.at( 1 ) == '\\' && constToken.tokenID.at( 2 ) == 'n' )
        returnResult.charValue = '\n' ;
      else if ( constToken.tokenID.at( 1 ) == '\\' && constToken.tokenID.at( 2 ) == 't' )
        returnResult.charValue = '\t' ;
      else
        returnResult.charValue = constToken.tokenID.at( 1 ) ;
    } 
    
    
    else if ( constToken.tokentype == CONSTANT_BOOL ) {
      returnResult.valueType = BOOL ;
      if ( constToken.tokenID.compare( "true" ) == 0 ) 
        returnResult.boolValue = true ;
      else
        returnResult.boolValue = false ;
    } 
    
    
    else if ( constToken.tokentype == CONSTANT_STRING ) {
      returnResult.valueType = STRING ;
      
      for ( int i = 1 ; i < constToken.tokenID.size()-1 ; i++ ) {
        if ( constToken.tokenID.at( i ) == '\\' ) {
          if ( ( i + 1 ) < constToken.tokenID.size()-1 ) {
            if ( constToken.tokenID.at( i+1 ) == 'n' ) {
              returnResult.stringValue = returnResult.stringValue + n ;
              i++ ;
            } 
            
            else if ( constToken.tokenID.at( i+1 ) == 't' ) {
              returnResult.stringValue = returnResult.stringValue + t ;
              i++ ;
            } 
            
            else {
              returnResult.stringValue = returnResult.stringValue + constToken.tokenID.at( i ) ;
            } 
          } 
          
          else {
            returnResult.stringValue = returnResult.stringValue + constToken.tokenID.at( i ) ;
          } 
        } 
        
        else {
          returnResult.stringValue = returnResult.stringValue + constToken.tokenID.at( i ) ;
        } 
      } 
    } 
  } 
  
  
  
  
  
  bool A_user_input( int &firstTokenStartLine, int &firstTokenStartColumn, bool &isQuit ) { 
    TOKENTYPE tokentype ; 
    int nextTokenIndex = -1 ; 
    int index = 0 ;
    LOCAL firstStack ;
    
    
    mCoutExpFalse = 0 ;
    mDoExp = true ;
    
    
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
        
        mUserInputType = STATEMENT ;
        
        
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
            } 
            
            
          } 
          
          
          
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
      
      nextTokenIndex = FindTokenInAllTokenStringList( firstTokenStartLine, firstTokenStartColumn ) ;   
      
      
      PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      
      
      if ( tokentype == SEMICOLON  || tokentype == IDENTIFIER || tokentype == PP 
           || tokentype == MM || Sign( tokentype ) 
           || tokentype == CONSTANT_INT || tokentype == CONSTANT_FLOAT 
           || tokentype == CONSTANT_CHAR || tokentype == CONSTANT_STRING
           || tokentype == CONSTANT_BOOL || tokentype == LP || tokentype == LBC 
           || tokentype == IF || tokentype == WHILE
           || tokentype == RETURN || tokentype == DO  ) {
        
        
        
        
        
        if ( gAllTokenStringList[nextTokenIndex].tokenID == "cout" 
             || !IsSystemSupportFunction( gAllTokenStringList[nextTokenIndex].tokenID ) ) {         
                   
          
          mUserInputType = STATEMENT ;
          
          
          if ( Statement( nextTokenIndex ) ) {
 
            
            index = FindTokenInAllTokenStringList( firstTokenStartLine, firstTokenStartColumn ) ;
            
            
            cout << "Statement executed ..." << endl ;         

          
          } 
        } 
      } 
       
      return true ;   
    } 
    
    return false ;
  } 

  
  
  bool Definition( int &nextTokenIndex ) {
    TOKENTYPE tokentype ; 
    IndentifierSymbolInfo newDefineSymbol ; 
    int arraySize = 0 ;
    
    
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
            newDefineSymbol.symbolValue.numValue = 1 ;
            newDefineSymbol.symbolValue.charValue = '1' ;
            newDefineSymbol.symbolValue.stringValue = "" ;
            newDefineSymbol.symbolValue.boolValue = false ;
            
            
            
            newDefineSymbol.symbolType = VARIABLE ;
            
            
            
            
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
                
                
                
                
                
                if ( mNewDefineSymbolList[i].symbolType == ARRAY ) {
                  SetArraySpace( mNewDefineSymbolList[i] ) ;
                } 
                
                
                
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

    return false ;
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
    
    
    PeekToken( tokentype ) ;
    
    
    if ( tokentype == LP ) {
      
      if ( Function_definition_without_ID( nextTokenIndex ) ) return true ;
      else return false ; 
    } 
    
    
    else if ( tokentype == LBK || tokentype == COMMA || tokentype == SEMICOLON )  {
      
      if ( Rest_of_declarators( nextTokenIndex ) ) return true ;
      else return false ; 
    } 
    
    
    else {
      
      GetToken( tokentype ) ;
      gAllTokenStringList.push_back( gToken ) ;

      
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
        
        
        mNewDefineSymbolList[0].symbolType = ARRAY ; 
        
        
        
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        if ( tokentype == CONSTANT_INT || tokentype == CONSTANT_FLOAT 
             || tokentype == CONSTANT_CHAR || tokentype == CONSTANT_STRING || tokentype == CONSTANT_BOOL ) {
          
          
          
          
          SetArrayInfo( mNewDefineSymbolList[0], gToken ) ;
          
           
          
          
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
                  
                  
                  
                  newDefineSymbol.symbolType = VARIABLE ;   
                    
                  newDefineSymbol.symbolStartLine = gToken.tokenLine ;  
                  newDefineSymbol.symbolStartColumn = gToken.tokenColumn ; 
                  
                  
                  mNewDefineFunc.localSymbolList.push_back( newDefineSymbol ) ;
                  IndentifierInit( newDefineSymbol ) ; 
                } 
                
                
                
                else {
                  
                  newDefineSymbol.name = gToken.tokenID ; 
                  newDefineSymbol.symbolStartLine = gToken.tokenLine ; 
                  newDefineSymbol.symbolStartColumn = gToken.tokenColumn ; 
                
                  
                  newDefineSymbol.symbolReturnType = 
                  mNewDefineSymbolList[mNewDefineSymbolList.size()-1].symbolReturnType ; 
                  
                  
                  
                  newDefineSymbol.symbolType = VARIABLE ;                  
                  
                  
                  mNewDefineSymbolList.push_back( newDefineSymbol ) ;  
                } 
                
                
                
                
                PeekToken( tokentype ) ;
                
                
                if ( tokentype == LBK ) {
                  
                  
                  
                  
                  mNewDefineSymbolList[mNewDefineSymbolList.size()-1].symbolType = ARRAY ;  
                  
                  
                  
                  
                  GetToken( tokentype ) ;
                  gAllTokenStringList.push_back( gToken ) ;
                  
                  
                  
                  GetToken( tokentype ) ;
                  gAllTokenStringList.push_back( gToken ) ;
                  
                  
                  if ( tokentype == CONSTANT_INT || tokentype == CONSTANT_FLOAT 
                       || tokentype == CONSTANT_CHAR || tokentype == CONSTANT_STRING 
                       || tokentype == CONSTANT_BOOL ) {
                    
                    
                    
                    
                    SetArrayInfo( mNewDefineSymbolList[mNewDefineSymbolList.size()-1], gToken ) ; 
                    
                     
                    
                    
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
              
              
              
              newDefineSymbol.symbolType = VARIABLE ; 
                
              newDefineSymbol.symbolStartLine = gToken.tokenLine ;  
              newDefineSymbol.symbolStartColumn = gToken.tokenColumn ; 
              
              
              mNewDefineFunc.localSymbolList.push_back( newDefineSymbol ) ;
              IndentifierInit( newDefineSymbol ) ; 
            } 
            
            
            
            else {
              
              newDefineSymbol.name = gToken.tokenID ; 
              newDefineSymbol.symbolStartLine = gToken.tokenLine ; 
              newDefineSymbol.symbolStartColumn = gToken.tokenColumn ; 
              
              
              
              newDefineSymbol.symbolReturnType = 
              mNewDefineSymbolList[mNewDefineSymbolList.size()-1].symbolReturnType ;
              
              
              
              newDefineSymbol.symbolType = VARIABLE ; 
              
              
              mNewDefineSymbolList.push_back( newDefineSymbol ) ; 
            } 
            
            
            
            
            PeekToken( tokentype ) ;
            
            
            if ( tokentype == LBK ) {
              
              
              
              
              mNewDefineSymbolList[mNewDefineSymbolList.size()-1].symbolType = ARRAY ;  
              
              
              
              
              GetToken( tokentype ) ;
              gAllTokenStringList.push_back( gToken ) ;
              
              
              
              GetToken( tokentype ) ;
              gAllTokenStringList.push_back( gToken ) ;
              
              
              if ( tokentype == CONSTANT_INT || tokentype == CONSTANT_FLOAT 
                   || tokentype == CONSTANT_CHAR || tokentype == CONSTANT_STRING 
                   || tokentype == CONSTANT_BOOL ) {
                     
                
                
                
                SetArrayInfo( mNewDefineSymbolList[mNewDefineSymbolList.size()-1], gToken ) ; 
                
                
                
                
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
      
       
      
      PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      
      
      
      if ( tokentype == LBK ) {
         
        
        
        mNewDefineSymbolList[0].symbolType = ARRAY ; 
        
        
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++ ;
        
        
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++ ;
        
        
        if ( tokentype == CONSTANT_INT || tokentype == CONSTANT_FLOAT 
             || tokentype == CONSTANT_CHAR || tokentype == CONSTANT_STRING || tokentype == CONSTANT_BOOL ) {
          
          
          
          SetArrayInfo( mNewDefineSymbolList[0], gToken ) ;
           
          
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex++ ;
          
          
          if ( tokentype == RBK ) {
            
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
            
            
            while ( tokentype == COMMA ) {
              
              
              PassTwoGetToken( nextTokenIndex, tokentype ) ;
              nextTokenIndex++ ;
              
              
              
              PassTwoGetToken( nextTokenIndex, tokentype ) ;
              nextTokenIndex++ ;
              
              
              if ( tokentype == IDENTIFIER ) {
                
                
                
                newDefineSymbol.name = gToken.tokenID ; 
                newDefineSymbol.symbolStartLine = gToken.tokenLine ; 
                newDefineSymbol.symbolStartColumn = gToken.tokenColumn ; 
              
                
                newDefineSymbol.symbolReturnType = 
                mNewDefineSymbolList[mNewDefineSymbolList.size()-1].symbolReturnType ; 
                
                
                
                newDefineSymbol.symbolType = VARIABLE ; 
                
                newDefineSymbol.symbolValue.numValue = 1 ;
                newDefineSymbol.symbolValue.charValue = '1' ;
                newDefineSymbol.symbolValue.stringValue = "" ;
                newDefineSymbol.symbolValue.boolValue = false ;          
                
                
                mNewDefineSymbolList.push_back( newDefineSymbol ) ;  
                
                
                
                PassTwoPeekToken( nextTokenIndex, tokentype ) ;
                
                
                if ( tokentype == LBK ) {
                  
                  
                  
                  mNewDefineSymbolList[mNewDefineSymbolList.size()-1].symbolType = ARRAY ;
                  
                  
                  
                  PassTwoGetToken( nextTokenIndex, tokentype ) ;
                  nextTokenIndex++ ;
                  
                  
                  
                  PassTwoGetToken( nextTokenIndex, tokentype ) ;
                  nextTokenIndex++ ;
                  
                  
                  if ( tokentype == CONSTANT_INT || tokentype == CONSTANT_FLOAT 
                       || tokentype == CONSTANT_CHAR || tokentype == CONSTANT_STRING 
                       || tokentype == CONSTANT_BOOL ) {
                      
                    
                    
                    SetArrayInfo( mNewDefineSymbolList[mNewDefineSymbolList.size()-1], gToken ) ;
                     
                    
                    
                    PassTwoGetToken( nextTokenIndex, tokentype ) ;
                    nextTokenIndex++ ;
                    
                    
                    if ( tokentype == RBK ) {
                      
                      PassTwoPeekToken( nextTokenIndex, tokentype ) ;
                    } 
                  } 
                } 
              } 
            } 
            
            
            PassTwoGetToken( nextTokenIndex, tokentype ) ;
            nextTokenIndex++ ;
            
            
            if ( tokentype == SEMICOLON ) {
              return true;
            } 
          } 
        } 
      } 
      
      
      else if ( tokentype == COMMA ) {
        
        while ( tokentype == COMMA ) {
          
          
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex++ ;
          
          
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex++ ;
          
          
          if ( tokentype == IDENTIFIER ) {
            
            
            

            newDefineSymbol.name = gToken.tokenID ; 
            newDefineSymbol.symbolStartLine = gToken.tokenLine ; 
            newDefineSymbol.symbolStartColumn = gToken.tokenColumn ; 
            
            
            
            newDefineSymbol.symbolReturnType = 
            mNewDefineSymbolList[mNewDefineSymbolList.size()-1].symbolReturnType ;
            
            
            
            newDefineSymbol.symbolType = VARIABLE ;
            
            newDefineSymbol.symbolValue.numValue = 1 ;
            newDefineSymbol.symbolValue.charValue = '1' ;
            newDefineSymbol.symbolValue.stringValue = "" ;
            newDefineSymbol.symbolValue.boolValue = false ;
                
            
            
            mNewDefineSymbolList.push_back( newDefineSymbol ) ; 
            
            
            
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
            
            
            if ( tokentype == LBK ) {
              
              
              
              mNewDefineSymbolList[mNewDefineSymbolList.size()-1].symbolType = ARRAY ;  
              
              
              
              PassTwoGetToken( nextTokenIndex, tokentype ) ;
              nextTokenIndex++ ;
              
              
              
              PassTwoGetToken( nextTokenIndex, tokentype ) ;
              nextTokenIndex++ ;
              
              
              if ( tokentype == CONSTANT_INT || tokentype == CONSTANT_FLOAT 
                   || tokentype == CONSTANT_CHAR || tokentype == CONSTANT_STRING 
                   || tokentype == CONSTANT_BOOL ) {
                          
                
                
                SetArrayInfo( mNewDefineSymbolList[mNewDefineSymbolList.size()-1], gToken ) ; 

                
                
                PassTwoGetToken( nextTokenIndex, tokentype ) ;
                nextTokenIndex++ ; 
                
                
                if ( tokentype == RBK ) {
                  
                  PassTwoPeekToken( nextTokenIndex, tokentype ) ;
                } 
              } 
            } 
          } 
        } 
        
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++ ;
        
        
        if ( tokentype == SEMICOLON ) {
          return true ;
        } 
      } 
      
      
      else {
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++ ;
        
        
        if ( tokentype == SEMICOLON ) return true ;
      } 
    } 
    
    return false ;
  } 
  
  
  
  bool Function_definition_without_ID( int &nextTokenIndex ) {
    TOKENTYPE tokentype ; 

    
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

    return false ;
  } 
  
  
  
  
  
  bool Formal_parameter_list( int &nextTokenIndex ) {
    TOKENTYPE tokentype ; 
    IndentifierSymbolInfo newDefineSymbol ;
    ArgsDefineInfo argDefineInfo ;

    
    
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
        
        
        
        argDefineInfo.argType = VARIABLE ;   
                
        
        mNewDefineFunc.localSymbolList.push_back( newDefineSymbol ) ;
        
        mNewDefineFunc.argDefineList.push_back( argDefineInfo ) ;
        
        IndentifierInit( newDefineSymbol ) ; 
        argDefineInfo.isReturn = false ; 
        
        
        
        PeekToken( tokentype ) ;
        
        if ( tokentype == LBK ) {
          
          
          
          
          mNewDefineFunc.argDefineList[mNewDefineFunc.argDefineList.size()-1].argType = ARRAY ; 
          
  
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
          
          
          if ( tokentype == CONSTANT_INT || tokentype == CONSTANT_CHAR 
               || tokentype == CONSTANT_FLOAT || tokentype == CONSTANT_BOOL 
               || tokentype == CONSTANT_STRING ) {
            
            
            
            
            SetArrayInfo( mNewDefineSymbolList[mNewDefineSymbolList.size()-1], gToken ) ; 
            
            
            
            
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
              
              
              
              argDefineInfo.argType =  VARIABLE ;  

              
              mNewDefineFunc.localSymbolList.push_back( newDefineSymbol ) ;
              
              mNewDefineFunc.argDefineList.push_back( argDefineInfo ) ;
                      
              IndentifierInit( newDefineSymbol ) ; 
              argDefineInfo.isReturn = false ; 
              
              
              
              PeekToken( tokentype ) ;
              
              if ( tokentype == LBK ) {
                
                
                
                mNewDefineFunc.argDefineList[mNewDefineFunc.argDefineList.size()-1].argType = ARRAY ; 
                
                
                
                
                GetToken( tokentype ) ;
                gAllTokenStringList.push_back( gToken ) ;
          
                
                GetToken( tokentype ) ;
                gAllTokenStringList.push_back( gToken ) ;
          
                
                if ( tokentype == CONSTANT_INT || tokentype == CONSTANT_CHAR 
                     || tokentype == CONSTANT_FLOAT || tokentype == CONSTANT_BOOL 
                     || tokentype == CONSTANT_STRING ) {
                       
                  
                  
                  
                  SetArrayInfo( mNewDefineSymbolList[mNewDefineSymbolList.size()-1], gToken ) ; 
                  
            
                  
                  
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
    
    return false ;
  } 
  
  
  bool Compound_statement( int &nextTokenIndex ) {
    TOKENTYPE tokentype ; 
    int startIndex = 0 ;  
    int callStackLastIndex ; 
    int localVarLastIndex ; 
    int c = 0 ;
    
    
    
    if ( !mIsCompute ) {
      if ( mUserInputType == DEFINEFUNCTION ) startIndex = mNewDefineFunc.localSymbolList.size() ;
      else startIndex = mNewDefineSymbolList.size() ;
    } 
    
    
    
    else {
      callStackLastIndex = gCallStack.size()-1 ;
      startIndex = gCallStack[callStackLastIndex].localList.size() ;
    } 
    
    
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
            
            if ( Declaration( nextTokenIndex, startIndex ) ) {
              
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
          
          if ( mUserInputType == DEFINEFUNCTION ) {
            for ( int i = mNewDefineFunc.localSymbolList.size()-1 ; i > startIndex-1 ; i-- ) {
              
              
              
              mNewDefineFunc.localSymbolList.erase( mNewDefineFunc.localSymbolList.begin() + i ) ;
            } 
          } 
          
          else {   
            for ( int i = mNewDefineSymbolList.size()-1 ; i > startIndex-1 ; i-- ) {
              
              
              
              
              mNewDefineSymbolList.erase( mNewDefineSymbolList.begin() + i ) ;
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
      
      
      PassTwoGetToken( nextTokenIndex, tokentype ) ;
      nextTokenIndex++ ;

      
      PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      
      while ( Type_specifier( tokentype ) || tokentype == SEMICOLON  
              || tokentype == IDENTIFIER || tokentype == PP || tokentype == MM 
              || Sign( tokentype ) || tokentype == CONSTANT_INT 
              || tokentype == CONSTANT_FLOAT || tokentype == CONSTANT_CHAR 
              || tokentype == CONSTANT_STRING || tokentype == CONSTANT_BOOL || tokentype == LP
              || tokentype == RETURN || tokentype == LBC || tokentype == IF || tokentype == WHILE 
              || tokentype == DO ) {
                
        
        if ( Type_specifier( tokentype ) ) {
          
          
          
          Declaration( nextTokenIndex, startIndex ) ;
          
          
          PassTwoPeekToken( nextTokenIndex, tokentype ) ;

        } 
        
        else {
          
          
          Statement( nextTokenIndex ) ;
          
          
          PassTwoPeekToken( nextTokenIndex, tokentype ) ;
        } 
      } 

      
      
      PassTwoGetToken( nextTokenIndex, tokentype ) ;
      nextTokenIndex++ ;
      
      
      
      
     
      
      callStackLastIndex = gCallStack.size()-1 ;
      c = callStackLastIndex ;
      localVarLastIndex = gCallStack[callStackLastIndex].localList.size()-1 ;  
    
      
      for ( int i = localVarLastIndex ; i > startIndex-1 ; i-- ) {
        gCallStack[c].localList.erase( 
                                       gCallStack[c].localList.begin() + i ) ;                     
      } 
        
      
      return true ;
    } 
    
    return false ;
  } 
  
  
  
  bool Declaration( int &nextTokenIndex, int startIndex ) {
    TOKENTYPE tokentype ;
    IndentifierSymbolInfo newDefineSymbol ;
    int index = 0 ;
    int callStackLastIndex = -1 ;
    int localVarLastIndex = -1 ;
    
    IndentifierInit( newDefineSymbol ) ;
    
    
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
            
            
            
            newDefineSymbol.symbolType = VARIABLE ;
            
            
            mNewDefineFunc.localSymbolList.push_back( newDefineSymbol ) ;
            IndentifierInit( newDefineSymbol ) ; 
          } 
          
          
          
          else {
            
            newDefineSymbol.name = gToken.tokenID ; 
            newDefineSymbol.symbolStartLine = gToken.tokenLine ; 
            newDefineSymbol.symbolStartColumn = gToken.tokenColumn ; 
            
            
            
            newDefineSymbol.symbolType = VARIABLE ;
            
            mNewDefineSymbolList.push_back( newDefineSymbol ) ;
            IndentifierInit( newDefineSymbol ) ; 
          } 
          
                
          
          if ( Rest_of_declarators( nextTokenIndex ) ) {
            
            
            
            if ( mUserInputType == DEFINEFUNCTION ) {
              
              for ( int i = 0 ; i < mNewDefineSymbolList.size() ; i++ ) {
                mNewDefineFunc.localSymbolList.push_back( newDefineSymbol ) ;
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
      
      
      PassTwoGetToken( nextTokenIndex, tokentype ) ;
      nextTokenIndex++ ;
      
      
      newDefineSymbol.symbolReturnType = tokentype ;

      
      
      PassTwoGetToken( nextTokenIndex, tokentype ) ;
      nextTokenIndex++ ;
      

      

      newDefineSymbol.name = gToken.tokenID ; 
      newDefineSymbol.symbolStartLine = gToken.tokenLine ; 
      newDefineSymbol.symbolStartColumn = gToken.tokenColumn ; 
      newDefineSymbol.symbolValue.numValue = 1 ;
      newDefineSymbol.symbolValue.stringValue = "" ;
      newDefineSymbol.symbolValue.charValue = '1' ;
      newDefineSymbol.symbolValue.boolValue = false ;
      
      
      
      newDefineSymbol.symbolType = VARIABLE ;
      
      mNewDefineSymbolList.push_back( newDefineSymbol ) ;
      IndentifierInit( newDefineSymbol ) ; 

      
      
      
      
      Rest_of_declarators( nextTokenIndex ) ;
   
      
      
      for ( int i = 0 ; i < mNewDefineSymbolList.size() ; i++ ) {
        
        
        
        if ( mNewDefineSymbolList[i].symbolType == ARRAY ) {
          SetArraySpace( mNewDefineSymbolList[i] ) ;
        } 
        
        InsertSymbolToLocalSymbolList( mNewDefineSymbolList[i], startIndex ) ;
      } 
      
      mNewDefineSymbolList.clear() ; 

      return true ;
    } 
    
    return false ;
  } 
  
  
  void SkipNoExeStatement( int &nextTokenIndex ) {
    int countBlock = 0 ;
    bool isDone = false ;
    
    while ( !isDone ) {
      if ( gAllTokenStringList[nextTokenIndex].tokentype == LBC ) {
        countBlock++ ;
      } 
      
      else if ( gAllTokenStringList[nextTokenIndex].tokentype == RBC ) {
        countBlock-- ;
        
        if ( countBlock == 0 ) isDone = true ;
      } 
      
      else if ( gAllTokenStringList[nextTokenIndex].tokentype == SEMICOLON && countBlock == 0 ) {
        isDone = true ;
      } 
      
      nextTokenIndex++ ;
    } 
  } 
  
  
  
  
  void SkipTrueStatementInRomce( int &nextTokenIndex ) {
    int count = 1 ;
    bool isDone = false ;
    
    while ( !isDone ) {
      
      if ( gAllTokenStringList[nextTokenIndex].tokentype == COLON ) {
        count-- ;
        
        if ( count == 0 ) isDone = true ;
      } 
      
      else if ( gAllTokenStringList[nextTokenIndex].tokentype == QMARK ) {
        count++ ;
      } 
         
      if ( !isDone ) nextTokenIndex++ ;    
      
    } 
  } 
  
  
  bool IsResultTrueOrNot( RESULT r ) {
    
    if ( r.valueType == INT || r.valueType == FLOAT ) {
      if ( r.numValue >= 1 ) return true ;
    } 
    
    else if ( r.valueType == BOOL ) {
      if ( r.boolValue ) return true ;
    } 
    
    return false ;
  } 

  
  bool Statement( int &nextTokenIndex ) {
    TOKENTYPE tokentype ;
    RESULT expValue ;
    int expStartIndex = 0 ;
    int stmtStartIndex = 0 ; 
    
    
    
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
        
        
        if ( Expression( nextTokenIndex, expValue ) ) {
          
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
          
          
          if ( Expression( nextTokenIndex, expValue ) ) {
            
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
          
          
          if ( Expression( nextTokenIndex, expValue ) ) {
            
            
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
          
          if ( Expression( nextTokenIndex, expValue ) ) {
            
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
              
              if ( Expression( nextTokenIndex, expValue ) ) {
                
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
      
      PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      
      
      if ( tokentype == SEMICOLON ) {
        
        
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++ ;
        
        return true ;
      } 
      
      
      else if ( tokentype == IDENTIFIER || tokentype == PP || tokentype == MM 
                || Sign( tokentype ) || tokentype == CONSTANT_INT 
                || tokentype == CONSTANT_FLOAT || tokentype == CONSTANT_CHAR 
                || tokentype == CONSTANT_STRING || tokentype == CONSTANT_BOOL
                || tokentype == LP ) {
        
        
        Expression( nextTokenIndex, expValue ) ;
          
        
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++ ;
      
        return true ;
      } 
      
      
      
      
      else if ( tokentype == RETURN ) {
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++ ;
        
        
        
        Expression( nextTokenIndex, expValue ) ;
        
        
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++ ;
            
        
        
        
        
     
        return true ;
      } 
      
      
      else if ( tokentype == LBC ) {
        
        
        Compound_statement( nextTokenIndex ) ; 
        return true ;
      } 
      
      
      else if ( tokentype == IF ) {
        
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++ ;
        
        
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++ ;
        
        
        Expression( nextTokenIndex, expValue ) ;
          
        
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++ ;
          
        
        
        if ( IsResultTrueOrNot( expValue ) ) {
          
          
          Statement( nextTokenIndex ) ;
          
          
          
          PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          
          
          
          if ( tokentype == ELSE ) {

            
            
            PassTwoGetToken( nextTokenIndex, tokentype ) ;
            nextTokenIndex++ ;
                    
            mDoExp = false ;
            mCoutExpFalse++ ; 
            Statement( nextTokenIndex ) ;
            mCoutExpFalse-- ;
            
            if ( mCoutExpFalse == 0 ) mDoExp = true ;
          } 
          
          return true ;
        } 
        
        
        else {
          
          
          
          mDoExp = false ;
          mCoutExpFalse++ ; 
          Statement( nextTokenIndex ) ;
          mCoutExpFalse-- ;
            
          if ( mCoutExpFalse == 0 ) mDoExp = true ;
          
          
          
          PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          
          
          if ( tokentype == ELSE ) {            
            
            
            PassTwoGetToken( nextTokenIndex, tokentype ) ;
            nextTokenIndex++ ; 
            
            
            
            Statement( nextTokenIndex ) ;       
          } 
          
          return true ;
        } 
      } 
      
      
      else if ( tokentype == WHILE ) {
        
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++ ;
        
        
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++ ;
        
        
        expStartIndex = nextTokenIndex ;
        
        
        
        Expression( nextTokenIndex, expValue ) ;
        
        if ( mDoExp ) {
          
          
          
          while ( IsResultTrueOrNot( expValue ) ) {
            
            
            PassTwoGetToken( nextTokenIndex, tokentype ) ;
            nextTokenIndex++ ;
            
            
            
            Statement( nextTokenIndex ) ;
             
            
            nextTokenIndex = expStartIndex ;
            
            
            
            Expression( nextTokenIndex, expValue ) ;
          } 
          
               
          
          
          
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex++ ;
          
          
          mDoExp = false ;
          mCoutExpFalse++ ;
          Statement( nextTokenIndex ) ;
          mCoutExpFalse-- ; 
          if ( mCoutExpFalse == 0 ) mDoExp = true ;
        } 
        
        
        else {
          
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex++ ;
          
          
          mDoExp = false ;
          mCoutExpFalse++ ;
          Statement( nextTokenIndex ) ;
          mCoutExpFalse-- ; 
          if ( mCoutExpFalse == 0 ) mDoExp = true ;
          
        } 
        
        
        return true ;
      } 
      
      
      else if ( tokentype == DO ) {
        
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++ ;
        
        
        stmtStartIndex = nextTokenIndex ;
        
        do {
          
          nextTokenIndex = stmtStartIndex ;
          
          
          
          Statement( nextTokenIndex ) ;
          
          
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex++ ;
          
          
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex++ ;
          
          
          
          Expression( nextTokenIndex, expValue ) ;
          
          
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex++ ;
          
          
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex++ ;
          
        } while ( IsResultTrueOrNot( expValue ) ) ;
      
        return true ;
      } 
    } 
    
    return false ;
  } 
  
  bool Expression( int &nextTokenIndex, RESULT &returnResult ) {
    TOKENTYPE tokentype ;
    RESULT basicExpValue ;
    
    
    if ( !mIsCompute ) {
      
      
      if ( Basic_expression( nextTokenIndex, basicExpValue ) ) {
        
        PeekToken( tokentype ) ;
        
        
        
        while ( tokentype == COMMA ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
          
          
          if ( Basic_expression( nextTokenIndex, basicExpValue ) ) {
            
            PeekToken( tokentype ) ;
          } 
          
          else return false ; 
        } 
        
        return true ;
      } 
      
      else return false ; 
    } 
    
    
    else {
      
      
      
      Basic_expression( nextTokenIndex, basicExpValue ) ;
      
      
      returnResult = basicExpValue ;
      
      
      
      PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      
      
      while ( tokentype == COMMA ) {
        
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++ ;
        
        
        
        Basic_expression( nextTokenIndex, basicExpValue ) ;
        
        
        returnResult = basicExpValue ;
        
        
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      } 
      
      return true ;
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
  
  bool Basic_expression( int &nextTokenIndex, RESULT &returnResult ) {
    TOKENTYPE tokentype ;
    TOKENTYPE operationValue ;
    TOKENTYPE signValue ;
    RESULT restIdBasicExpValue ;
    RESULT restPPMMexpValue ;
    RESULT signedExpValue ;
    RESULT rrValue ;
    RESULT expValue ;
    RESULT r ;
    
    IndentifierSymbolInfo newDefineSymbol ;
    int index = 0 ;
    
    
    bool isIdGlobal = false ;
    int idIndex = -1 ;
    RESULT preValue ;

    
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
            
            index = FindTokenInSymbolList( gToken.tokenID, mNewDefineSymbolList ) ;
            
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
        } 
        
          
        
        if ( Rest_of_Identifier_started_basic_exp( nextTokenIndex, restIdBasicExpValue, preValue ) ) 
          return true ;
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
              
              index = FindTokenInSymbolList( gToken.tokenID, mNewDefineSymbolList ) ;
              
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
          } 
          
          
          
          if ( Rest_of_PPMM_Identifier_started_basic_exp( nextTokenIndex, restPPMMexpValue, 
                                                          preValue, operationValue ) ) 
            return true ;
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
        
        
        if ( Signed_unary_exp( nextTokenIndex, signedExpValue ) ) {
          
          if ( Romce_and_romloe( nextTokenIndex, rrValue, preValue ) ) return true ;
          else return false ; 
        } 
        
        else return false ; 
      } 
      
      
      else if ( tokentype == CONSTANT_INT || tokentype == CONSTANT_FLOAT 
                || tokentype == CONSTANT_CHAR || tokentype == CONSTANT_STRING 
                || tokentype == CONSTANT_BOOL ) {
        
        if ( Romce_and_romloe( nextTokenIndex, rrValue, preValue ) ) return true ;
        else return false ; 
      } 
      
      
      else if ( tokentype == LP ) {
        
        if ( Expression( nextTokenIndex, expValue ) ) {
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
          
          
          if ( tokentype == RP ) {
            
            if ( Romce_and_romloe( nextTokenIndex, rrValue, preValue ) ) return true ;
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
      
      
      PassTwoGetToken( nextTokenIndex, tokentype ) ;
      nextTokenIndex++ ;
      
      
      if ( tokentype == IDENTIFIER ) {
        
        
        
        
        preValue.valueType = IDENTIFIER ;
          
        
        preValue.idName = gToken.tokenID ;
        
        
        
        
        
        Rest_of_Identifier_started_basic_exp( nextTokenIndex, restIdBasicExpValue, preValue ) ;
        
        
        returnResult = restIdBasicExpValue ;
        
        return true ;
      } 
      
      
      else if ( tokentype == PP || tokentype == MM ) {
        
        operationValue = tokentype ;
         
        
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++ ;
        
        
        
        
        preValue.valueType = IDENTIFIER ;
          
        
        preValue.idName = gToken.tokenID ;
        
        
        
        
        
        
        Rest_of_PPMM_Identifier_started_basic_exp( nextTokenIndex, restPPMMexpValue, 
                                                   preValue, operationValue ) ;
        
        
        returnResult = restPPMMexpValue ;
         
        return true ; 
      } 
      
      
      else if ( Sign( tokentype ) ) {
        
        
        signValue = tokentype ;
        
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
        
        while ( Sign( tokentype ) ) {
          
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex++;
          
          
          if ( signValue == PLUS && tokentype == MINUS ) signValue = MINUS ;
          
          else if ( signValue == MINUS && tokentype == MINUS ) signValue = PLUS ;
          else if ( signValue == TRUE && tokentype == NOT ) signValue = NOT ;
          else if ( signValue == NOT && tokentype == NOT ) signValue = TRUE ;
          
          
          PassTwoPeekToken( nextTokenIndex, tokentype ) ;
        } 
        
        
        
        
        Signed_unary_exp( nextTokenIndex, signedExpValue ) ;
        
        
        
        if ( signedExpValue.valueType == INT || signedExpValue.valueType == FLOAT ) {
          if ( signValue == MINUS ) signedExpValue.numValue = signedExpValue.numValue * ( -1 ) ;
 
          
          if ( signedExpValue.valueType == INT ) {
            preValue.valueType = INT ;
            preValue.numValue = signedExpValue.numValue ;
          } 
          
          
          else {
            preValue.valueType = FLOAT ;
            preValue.numValue = signedExpValue.numValue ;
          } 
        } 
        
        else if ( signedExpValue.valueType == CHAR ) {
          if ( signValue == MINUS ) preValue.numValue = preValue.charValue * ( -1 ) ;
          else preValue.numValue = preValue.charValue ;
          preValue.valueType = INT ;
        } 
        
        else if ( signedExpValue.valueType == BOOL ) {
          if ( signValue == NOT ) {
            if ( signedExpValue.boolValue ) {
              signedExpValue.boolValue = false ;
              preValue.boolValue = signedExpValue.boolValue ;
            } 
            
            else {
              signedExpValue.boolValue = true ;
              preValue.boolValue = signedExpValue.boolValue ;
            } 
            
          } 
          
          preValue.valueType = BOOL ;
        } 
        
        
        
        
        Romce_and_romloe( nextTokenIndex, rrValue, preValue ) ;
        
        
        returnResult = rrValue ;
        
        return true ; 
      } 
      
      
      else if ( tokentype == CONSTANT_INT || tokentype == CONSTANT_FLOAT 
                || tokentype == CONSTANT_CHAR || tokentype == CONSTANT_STRING 
                || tokentype == CONSTANT_BOOL ) {
        
        
        ReturnConstValue( r, gToken ) ;
        
        
        
        Romce_and_romloe( nextTokenIndex, rrValue, r ) ;
        
        
        
        returnResult = rrValue ;
         
        return true ;
      } 
      
      
      else if ( tokentype == LP ) {
        
        
        Expression( nextTokenIndex, expValue ) ;
        
        
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++;
        
        
        
        
        Romce_and_romloe( nextTokenIndex, rrValue, expValue ) ;
        
        
        returnResult = rrValue ;
        
        return true ;
      } 
    } 
    
    return false ;
  } 
  
  void AssignArrayValue( bool isIdGlobal, int idIndex, int arrayIndex, RESULT assignValue ) {
    int arrayInfoEntry ;
    int callStackLastIndex ;
    ArrayInfo a ;
    
    if ( isIdGlobal ) {
      
      arrayInfoEntry = gGlobalSymbolList[idIndex].arrayEntry ;
      
      
      a.arrayTable = gArrayInfoTable[arrayInfoEntry].arrayTable ;
      a.arrayEntry = gArrayInfoTable[arrayInfoEntry].arrayEntry ;
    } 
    
    else {
      
      callStackLastIndex = gCallStack.size()-1;
      
      
      arrayInfoEntry = gCallStack[callStackLastIndex].localList[idIndex].arrayEntry ;
      
      
      a.arrayTable = gArrayInfoTable[arrayInfoEntry].arrayTable ;
      a.arrayEntry = gArrayInfoTable[arrayInfoEntry].arrayEntry ;
    } 
    
    
    if ( a.arrayTable == 1 ) {
      if ( assignValue.valueType == INT || assignValue.valueType == FLOAT ) {
        gArrayIntTable[a.arrayEntry+arrayIndex] = assignValue.numValue ;
      } 
    } 
    
    
    else if ( a.arrayTable == 2 ) {
      if ( assignValue.valueType == INT || assignValue.valueType == FLOAT ) {
        gArrayFloatTable[a.arrayEntry+arrayIndex] = assignValue.numValue ;
      } 
    } 
    
    
    else if ( a.arrayTable == 3 ) {
      if ( assignValue.valueType == CHAR ) {
        gArrayCharTable[a.arrayEntry+arrayIndex] = assignValue.charValue ;
      } 
    } 
    
    
    else if ( a.arrayTable == 4 ) {
      if ( assignValue.valueType == BOOL ) {
        gArrayBoolTable[a.arrayEntry+arrayIndex] = assignValue.boolValue ;
      } 
    } 
    
    
    else if ( a.arrayTable == 5 ) {
      if ( assignValue.valueType == STRING ) {
        gArrayStringTable[a.arrayEntry+arrayIndex] = assignValue.stringValue ;
      } 
    } 
  } 
  
  void AssignValue( bool isIdGlobal, int idIndex, RESULT assignValue ) {
    int callStackLastIndex ;
    TOKENTYPE idType ;
    
    if ( isIdGlobal ) {
      idType = gGlobalSymbolList[idIndex].symbolReturnType ;
      
      if ( idType == INT || idType == FLOAT ) {
        if ( assignValue.valueType == INT || assignValue.valueType == FLOAT ) {
          gGlobalSymbolList[idIndex].symbolValue.numValue = assignValue.numValue ;
        } 
      } 
      
      else if ( idType == CHAR ) {
        if ( assignValue.valueType == CHAR ) {
          gGlobalSymbolList[idIndex].symbolValue.charValue = assignValue.charValue ;
        } 
      } 
      
      else if ( idType == BOOL ) {
        if ( assignValue.valueType == BOOL ) {
          gGlobalSymbolList[idIndex].symbolValue.boolValue = assignValue.boolValue ;
        } 
      } 
      
      else if ( idType == STRING ) {
        if ( assignValue.valueType == STRING ) {
          gGlobalSymbolList[idIndex].symbolValue.stringValue = assignValue.stringValue ;
        } 
      } 
    } 
    
    else {
      callStackLastIndex = gCallStack.size()-1 ;
      
      idType = gCallStack[callStackLastIndex].localList[idIndex].symbolReturnType ;
      
      if ( idType == INT || idType == FLOAT ) {
        if ( assignValue.valueType == INT || assignValue.valueType == FLOAT ) {
          gCallStack[callStackLastIndex].localList[idIndex].symbolValue.numValue = assignValue.numValue ;
        } 
      } 
      
      else if ( idType == CHAR ) {
        if ( assignValue.valueType == CHAR ) {
          gCallStack[callStackLastIndex].localList[idIndex].symbolValue.charValue = assignValue.charValue ;
        } 
      } 
      
      else if ( idType == BOOL ) {
        if ( assignValue.valueType == BOOL ) {
          gCallStack[callStackLastIndex].localList[idIndex].symbolValue.boolValue = assignValue.boolValue ;
        } 
      } 
      
      else if ( idType == STRING ) {
        if ( assignValue.valueType == STRING ) {
          gCallStack[callStackLastIndex].localList[idIndex].symbolValue.stringValue 
          = assignValue.stringValue ;
        } 
      } 
    } 
  } 
  
  void TE_Array_Compute( RESULT &returnResult, bool isIdGlobal, 
                         int idIndex, int arrayIndex, RESULT expValue ) {
    int arrayInfoEntry  = 0 ;
    int callStackLastIndex = 0 ;
    ArrayInfo a ;
    
    if ( isIdGlobal ) {
      
      arrayInfoEntry = gGlobalSymbolList[idIndex].arrayEntry ;
      
      
      a.arrayTable = gArrayInfoTable[arrayInfoEntry].arrayTable ;
      a.arrayEntry = gArrayInfoTable[arrayInfoEntry].arrayEntry ;
    } 
    
    else {
      
      callStackLastIndex = gCallStack.size()-1;
      
      
      arrayInfoEntry = gCallStack[callStackLastIndex].localList[idIndex].arrayEntry ;
      
      
      a.arrayTable = gArrayInfoTable[arrayInfoEntry].arrayTable ;
      a.arrayEntry = gArrayInfoTable[arrayInfoEntry].arrayEntry ;
    } 
    
    
    if ( a.arrayTable == 1 ) {
      if ( expValue.valueType == INT || expValue.valueType == FLOAT ) {
        gArrayIntTable[a.arrayEntry+arrayIndex] = 
        gArrayIntTable[a.arrayEntry+arrayIndex] * expValue.numValue ;
        
        returnResult.valueType = INT ;
        returnResult.numValue =  gArrayIntTable[a.arrayEntry+arrayIndex] ;
      } 
    } 
    
    
    else if ( a.arrayTable == 2 ) {
      if ( expValue.valueType == INT || expValue.valueType == FLOAT ) {
        gArrayFloatTable[a.arrayEntry+arrayIndex] = 
        gArrayFloatTable[a.arrayEntry+arrayIndex] * expValue.numValue ;
        
        returnResult.valueType = FLOAT ;
        returnResult.numValue = gArrayFloatTable[a.arrayEntry+arrayIndex] ;
      } 
    } 
  } 
  
  void TE_Compute( RESULT &returnResult, bool isIdGlobal, int idIndex, RESULT expValue ) {
    int callStackLastIndex = 0 ;
    
    TOKENTYPE idType ;
    
    if ( isIdGlobal ) {
      idType = gGlobalSymbolList[idIndex].symbolReturnType ;
      
      if ( idType == INT || idType == FLOAT ) {
        if ( expValue.valueType == INT || expValue.valueType == FLOAT ) {
          gGlobalSymbolList[idIndex].symbolValue.numValue = 
          gGlobalSymbolList[idIndex].symbolValue.numValue * expValue.numValue ;
          
          if ( idType == INT ) returnResult.valueType = INT ;
          else returnResult.valueType = FLOAT ;
          returnResult.numValue = gGlobalSymbolList[idIndex].symbolValue.numValue ;
        } 
      } 
    } 
    
    else {
      callStackLastIndex = gCallStack.size()-1 ;
      
      idType = gCallStack[callStackLastIndex].localList[idIndex].symbolReturnType ;
      
      if ( idType == INT || idType == FLOAT ) {
        if ( expValue.valueType == INT || expValue.valueType == FLOAT ) {
          gCallStack[callStackLastIndex].localList[idIndex].symbolValue.numValue = 
          gCallStack[callStackLastIndex].localList[idIndex].symbolValue.numValue * expValue.numValue ;
          
          if ( idType == INT ) returnResult.valueType = INT ;
          else returnResult.valueType = FLOAT ;
          returnResult.numValue = gCallStack[callStackLastIndex].localList[idIndex].symbolValue.numValue ;
        } 
      } 
    } 
  } 
  
  void DE_Array_Compute( RESULT &returnResult, bool isIdGlobal, 
                         int idIndex, int arrayIndex, RESULT expValue ) {
    int arrayInfoEntry = 0;
    int callStackLastIndex = 0 ;
    ArrayInfo a ;
    
    if ( isIdGlobal ) {
      
      arrayInfoEntry = gGlobalSymbolList[idIndex].arrayEntry ;
      
      
      a.arrayTable = gArrayInfoTable[arrayInfoEntry].arrayTable ;
      a.arrayEntry = gArrayInfoTable[arrayInfoEntry].arrayEntry ;
    } 
    
    else {
      
      callStackLastIndex = gCallStack.size()-1;
      
      
      arrayInfoEntry = gCallStack[callStackLastIndex].localList[idIndex].arrayEntry ;
      
      
      a.arrayTable = gArrayInfoTable[arrayInfoEntry].arrayTable ;
      a.arrayEntry = gArrayInfoTable[arrayInfoEntry].arrayEntry ;
    } 
    
    
    if ( a.arrayTable == 1 ) {
      if ( expValue.valueType == INT || expValue.valueType == FLOAT ) {
        gArrayIntTable[a.arrayEntry+arrayIndex] = 
        gArrayIntTable[a.arrayEntry+arrayIndex] / expValue.numValue ;
        
        returnResult.valueType = INT ;
        returnResult.numValue =  gArrayIntTable[a.arrayEntry+arrayIndex] ;
      } 
    } 
    
    
    else if ( a.arrayTable == 2 ) {
      if ( expValue.valueType == INT || expValue.valueType == FLOAT ) {
        gArrayFloatTable[a.arrayEntry+arrayIndex] = 
        gArrayFloatTable[a.arrayEntry+arrayIndex] / expValue.numValue ;
        
        returnResult.valueType = FLOAT ;
        returnResult.numValue = gArrayFloatTable[a.arrayEntry+arrayIndex] ;
      } 
    } 
  } 
  
  void DE_Compute( RESULT &returnResult, bool isIdGlobal, int idIndex, RESULT expValue ) {
    int callStackLastIndex ;
    
    TOKENTYPE idType ;
    
    if ( isIdGlobal ) {
      idType = gGlobalSymbolList[idIndex].symbolReturnType ;
      
      if ( idType == INT || idType == FLOAT ) {
        if ( expValue.valueType == INT || expValue.valueType == FLOAT ) {
          gGlobalSymbolList[idIndex].symbolValue.numValue = 
          gGlobalSymbolList[idIndex].symbolValue.numValue / expValue.numValue ;
          
          if ( idType == INT ) returnResult.valueType = INT ;
          else returnResult.valueType = FLOAT ;
          returnResult.numValue = gGlobalSymbolList[idIndex].symbolValue.numValue ;
        } 
      } 
    } 
    
    else {
      callStackLastIndex = gCallStack.size()-1 ;
      
      idType = gCallStack[callStackLastIndex].localList[idIndex].symbolReturnType ;
      
      if ( idType == INT || idType == FLOAT ) {
        if ( expValue.valueType == INT || expValue.valueType == FLOAT ) {
          gCallStack[callStackLastIndex].localList[idIndex].symbolValue.numValue = 
          gCallStack[callStackLastIndex].localList[idIndex].symbolValue.numValue / expValue.numValue ;
          
          if ( idType == INT ) returnResult.valueType = INT ;
          else returnResult.valueType = FLOAT ;
          returnResult.numValue = gCallStack[callStackLastIndex].localList[idIndex].symbolValue.numValue ;
        } 
      } 
    } 
  } 
  
  void RE_Array_Compute( RESULT &returnResult, bool isIdGlobal, 
                         int idIndex, int arrayIndex, RESULT expValue ) {
    int arrayInfoEntry = 0;
    int callStackLastIndex = 0 ;
    ArrayInfo a ;
    int num1 = 0 ;
    int num2 = 0 ;
    
    if ( isIdGlobal ) {
      
      arrayInfoEntry = gGlobalSymbolList[idIndex].arrayEntry ;
      
      
      a.arrayTable = gArrayInfoTable[arrayInfoEntry].arrayTable ;
      a.arrayEntry = gArrayInfoTable[arrayInfoEntry].arrayEntry ;
    } 
    
    else {
      
      callStackLastIndex = gCallStack.size()-1;
      
      
      arrayInfoEntry = gCallStack[callStackLastIndex].localList[idIndex].arrayEntry ;
      
      
      a.arrayTable = gArrayInfoTable[arrayInfoEntry].arrayTable ;
      a.arrayEntry = gArrayInfoTable[arrayInfoEntry].arrayEntry ;
    } 
    
    
    if ( a.arrayTable == 1 ) {
      
      
      if ( expValue.valueType == INT  ) {
        num1 = gArrayIntTable[a.arrayEntry+arrayIndex] ;
        num2 = expValue.numValue ;
        
        gArrayIntTable[a.arrayEntry+arrayIndex] = num1 % num2 ;
        
        returnResult.valueType = INT ;
        returnResult.numValue =  gArrayIntTable[a.arrayEntry+arrayIndex] ;
      } 
    } 
    
  } 
  
  void RE_Compute( RESULT &returnResult, bool isIdGlobal, int idIndex, RESULT expValue ) {
    int callStackLastIndex = 0 ;
    int num1, num2 = 0 ;
    
    TOKENTYPE idType ;
    
    if ( isIdGlobal ) {
      idType = gGlobalSymbolList[idIndex].symbolReturnType ;
      
      if ( idType == INT  ) {
        if ( expValue.valueType == INT ) {
          num1 = gGlobalSymbolList[idIndex].symbolValue.numValue ;
          num2 = expValue.numValue ;
          gGlobalSymbolList[idIndex].symbolValue.numValue = num1 % num2 ;
          
          returnResult.valueType = INT ;
          returnResult.numValue = gGlobalSymbolList[idIndex].symbolValue.numValue ;
        } 
      } 
    } 
    
    else {
      callStackLastIndex = gCallStack.size()-1 ;
      
      idType = gCallStack[callStackLastIndex].localList[idIndex].symbolReturnType ;
      
      if ( idType == INT ) {
        if ( expValue.valueType == INT ) {
          num1 = gGlobalSymbolList[idIndex].symbolValue.numValue ;
          num2 = expValue.numValue ;
          
          gCallStack[callStackLastIndex].localList[idIndex].symbolValue.numValue = num1 % num2 ;
          
          returnResult.valueType = INT ;
          returnResult.numValue = gCallStack[callStackLastIndex].localList[idIndex].symbolValue.numValue ;
        } 
      } 
    } 
  } 
  
  void PE_Array_Compute( RESULT &returnResult, bool isIdGlobal, 
                         int idIndex, int arrayIndex, RESULT expValue ) {
    int arrayInfoEntry = 0;
    int callStackLastIndex = 0;
    ArrayInfo a ;
    
    if ( isIdGlobal ) {
      
      arrayInfoEntry = gGlobalSymbolList[idIndex].arrayEntry ;
      
      
      a.arrayTable = gArrayInfoTable[arrayInfoEntry].arrayTable ;
      a.arrayEntry = gArrayInfoTable[arrayInfoEntry].arrayEntry ;
    } 
    
    else {
      
      callStackLastIndex = gCallStack.size()-1;
      
      
      arrayInfoEntry = gCallStack[callStackLastIndex].localList[idIndex].arrayEntry ;
      
      
      a.arrayTable = gArrayInfoTable[arrayInfoEntry].arrayTable ;
      a.arrayEntry = gArrayInfoTable[arrayInfoEntry].arrayEntry ;
    } 
    
    
    if ( a.arrayTable == 1 ) {
      if ( expValue.valueType == INT || expValue.valueType == FLOAT ) {
        gArrayIntTable[a.arrayEntry+arrayIndex] = 
        gArrayIntTable[a.arrayEntry+arrayIndex] + expValue.numValue ;
        
        returnResult.valueType = INT ;
        returnResult.numValue =  gArrayIntTable[a.arrayEntry+arrayIndex] ;
      } 
    } 
    
    
    else if ( a.arrayTable == 2 ) {
      if ( expValue.valueType == INT || expValue.valueType == FLOAT ) {
        gArrayFloatTable[a.arrayEntry+arrayIndex] = 
        gArrayFloatTable[a.arrayEntry+arrayIndex] + expValue.numValue ;
        
        returnResult.valueType = FLOAT ;
        returnResult.numValue = gArrayFloatTable[a.arrayEntry+arrayIndex] ;
      } 
    } 
    
    
    else if ( a.arrayTable == 5 ) {
      
      returnResult.valueType = STRING ;
      
      if ( expValue.valueType == INT ) {
        gArrayStringTable[a.arrayEntry+arrayIndex] = 
        gArrayStringTable[a.arrayEntry+arrayIndex] + IntToString( expValue.numValue ) ;

        returnResult.stringValue = gArrayStringTable[a.arrayEntry+arrayIndex] ;
      } 
      
      else if ( expValue.valueType == FLOAT ) {
        gArrayStringTable[a.arrayEntry+arrayIndex] = 
        gArrayStringTable[a.arrayEntry+arrayIndex] + DoubleToString( expValue.numValue ) ;

        returnResult.stringValue = gArrayStringTable[a.arrayEntry+arrayIndex] ;
      } 
      
      else if ( expValue.valueType == CHAR ) {
        gArrayStringTable[a.arrayEntry+arrayIndex] = 
        gArrayStringTable[a.arrayEntry+arrayIndex] + expValue.charValue ;

        returnResult.stringValue = gArrayStringTable[a.arrayEntry+arrayIndex] ;
      } 
      
      else if ( expValue.valueType == BOOL ) {
        if ( expValue.boolValue ) {
          gArrayStringTable[a.arrayEntry+arrayIndex] = 
          gArrayStringTable[a.arrayEntry+arrayIndex] + "true" ;
        } 
        
        else {
          gArrayStringTable[a.arrayEntry+arrayIndex] = 
          gArrayStringTable[a.arrayEntry+arrayIndex] + "false" ;
        } 
        
        returnResult.stringValue = gArrayStringTable[a.arrayEntry+arrayIndex] ;
      } 
      
      else if ( expValue.valueType == STRING ) {
        gArrayStringTable[a.arrayEntry+arrayIndex] = 
        gArrayStringTable[a.arrayEntry+arrayIndex] + expValue.stringValue ;

        returnResult.stringValue = gArrayStringTable[a.arrayEntry+arrayIndex] ;
      } 
    } 
    
  } 
  
  void PE_Compute( RESULT &returnResult, bool isIdGlobal, int idIndex, RESULT expValue ) {
    int callStackLastIndex = 0;
    
    TOKENTYPE idType ;
    
    if ( isIdGlobal ) {
      idType = gGlobalSymbolList[idIndex].symbolReturnType ;
      
      if ( idType == INT || idType == FLOAT ) {
        if ( expValue.valueType == INT || expValue.valueType == FLOAT ) {
          gGlobalSymbolList[idIndex].symbolValue.numValue = 
          gGlobalSymbolList[idIndex].symbolValue.numValue + expValue.numValue ;
          
          if ( idType == INT ) returnResult.valueType = INT ;
          else returnResult.valueType = FLOAT ;
          returnResult.numValue = gGlobalSymbolList[idIndex].symbolValue.numValue ;
        } 
      } 
      
      else if ( idType == STRING ) {
        returnResult.valueType = STRING ;
        
        if ( expValue.valueType == INT ) {
          gGlobalSymbolList[idIndex].symbolValue.stringValue =
          gGlobalSymbolList[idIndex].symbolValue.stringValue + IntToString( expValue.numValue ) ;
          
          returnResult.stringValue = gGlobalSymbolList[idIndex].symbolValue.stringValue ;
        } 
        
        else if ( expValue.valueType == FLOAT ) {
          gGlobalSymbolList[idIndex].symbolValue.stringValue =
          gGlobalSymbolList[idIndex].symbolValue.stringValue + DoubleToString( expValue.numValue ) ;
          
          returnResult.stringValue = gGlobalSymbolList[idIndex].symbolValue.stringValue ;
        } 
        
        else if ( expValue.valueType == CHAR ) {
          gGlobalSymbolList[idIndex].symbolValue.stringValue =
          gGlobalSymbolList[idIndex].symbolValue.stringValue + expValue.charValue ;
          
          returnResult.stringValue = gGlobalSymbolList[idIndex].symbolValue.stringValue ;
        } 
        
        else if ( expValue.valueType == BOOL ) {
          if ( expValue.boolValue ) {
            gGlobalSymbolList[idIndex].symbolValue.stringValue =
            gGlobalSymbolList[idIndex].symbolValue.stringValue + "true" ;
          } 
          
          else {
            gGlobalSymbolList[idIndex].symbolValue.stringValue =
            gGlobalSymbolList[idIndex].symbolValue.stringValue + "false" ;
          } 
                 
          returnResult.stringValue = gGlobalSymbolList[idIndex].symbolValue.stringValue ;
        } 
        
        else if ( expValue.valueType == STRING ) {
          gGlobalSymbolList[idIndex].symbolValue.stringValue =
          gGlobalSymbolList[idIndex].symbolValue.stringValue + expValue.stringValue ;
          
          returnResult.stringValue = gGlobalSymbolList[idIndex].symbolValue.stringValue ;
        } 
      } 
    } 
    
    else {
      callStackLastIndex = gCallStack.size()-1 ;
      
      idType = gCallStack[callStackLastIndex].localList[idIndex].symbolReturnType ;
      
      if ( idType == INT || idType == FLOAT ) {
        if ( expValue.valueType == INT || expValue.valueType == FLOAT ) {
          gCallStack[callStackLastIndex].localList[idIndex].symbolValue.numValue = 
          gCallStack[callStackLastIndex].localList[idIndex].symbolValue.numValue + expValue.numValue ;
          
          if ( idType == INT ) returnResult.valueType = INT ;
          else returnResult.valueType = FLOAT ;
          returnResult.numValue = gCallStack[callStackLastIndex].localList[idIndex].symbolValue.numValue ;
        } 
      } 
      
      else if ( idType == STRING ) {
        returnResult.valueType = STRING ;
        
        if ( expValue.valueType == INT ) {
          gCallStack[callStackLastIndex].localList[idIndex].symbolValue.stringValue = 
          gCallStack[callStackLastIndex].localList[idIndex].symbolValue.stringValue 
          + IntToString( expValue.numValue ) ;
          
          returnResult.stringValue = 
          gCallStack[callStackLastIndex].localList[idIndex].symbolValue.stringValue ;
        } 
        
        else if ( expValue.valueType == FLOAT ) {
          gCallStack[callStackLastIndex].localList[idIndex].symbolValue.stringValue = 
          gCallStack[callStackLastIndex].localList[idIndex].symbolValue.stringValue 
          + DoubleToString( expValue.numValue ) ;
          
          returnResult.stringValue = 
          gCallStack[callStackLastIndex].localList[idIndex].symbolValue.stringValue ;
        } 
        
        else if ( expValue.valueType == CHAR ) {
          gCallStack[callStackLastIndex].localList[idIndex].symbolValue.stringValue = 
          gCallStack[callStackLastIndex].localList[idIndex].symbolValue.stringValue + expValue.charValue ;
          
          returnResult.stringValue = 
          gCallStack[callStackLastIndex].localList[idIndex].symbolValue.stringValue ;
        } 
        
        else if ( expValue.valueType == BOOL ) {
          if ( expValue.boolValue ) {
            gCallStack[callStackLastIndex].localList[idIndex].symbolValue.stringValue = 
            gCallStack[callStackLastIndex].localList[idIndex].symbolValue.stringValue + "true" ;
          } 
          
          else {
            gCallStack[callStackLastIndex].localList[idIndex].symbolValue.stringValue = 
            gCallStack[callStackLastIndex].localList[idIndex].symbolValue.stringValue + "false" ;
          } 
                 
          returnResult.stringValue = 
          gCallStack[callStackLastIndex].localList[idIndex].symbolValue.stringValue ;
        } 
        
        else if ( expValue.valueType == STRING ) {
          gCallStack[callStackLastIndex].localList[idIndex].symbolValue.stringValue = 
          gCallStack[callStackLastIndex].localList[idIndex].symbolValue.stringValue + expValue.stringValue ;
          
          returnResult.stringValue = 
          gCallStack[callStackLastIndex].localList[idIndex].symbolValue.stringValue ;
        } 
      } 
    } 
  } 
  
  void ME_Array_Compute( RESULT &returnResult, bool isIdGlobal, 
                         int idIndex, int arrayIndex, RESULT expValue ) {
    int arrayInfoEntry = 0 ;
    int callStackLastIndex = 0 ;
    ArrayInfo a ;
    
    if ( isIdGlobal ) {
      
      arrayInfoEntry = gGlobalSymbolList[idIndex].arrayEntry ;
      
      
      a.arrayTable = gArrayInfoTable[arrayInfoEntry].arrayTable ;
      a.arrayEntry = gArrayInfoTable[arrayInfoEntry].arrayEntry ;
    } 
    
    else {
      
      callStackLastIndex = gCallStack.size()-1;
      
      
      arrayInfoEntry = gCallStack[callStackLastIndex].localList[idIndex].arrayEntry ;
      
      
      a.arrayTable = gArrayInfoTable[arrayInfoEntry].arrayTable ;
      a.arrayEntry = gArrayInfoTable[arrayInfoEntry].arrayEntry ;
    } 
    
    
    if ( a.arrayTable == 1 ) {
      if ( expValue.valueType == INT || expValue.valueType == FLOAT ) {
        gArrayIntTable[a.arrayEntry+arrayIndex] = 
        gArrayIntTable[a.arrayEntry+arrayIndex] - expValue.numValue ;
        
        returnResult.valueType = INT ;
        returnResult.numValue =  gArrayIntTable[a.arrayEntry+arrayIndex] ;
      } 
    } 
    
    
    else if ( a.arrayTable == 2 ) {
      if ( expValue.valueType == INT || expValue.valueType == FLOAT ) {
        gArrayFloatTable[a.arrayEntry+arrayIndex] = 
        gArrayFloatTable[a.arrayEntry+arrayIndex] - expValue.numValue ;
        
        returnResult.valueType = FLOAT ;
        returnResult.numValue = gArrayFloatTable[a.arrayEntry+arrayIndex] ;
      } 
    } 
  } 
  
  void ME_Compute( RESULT &returnResult, bool isIdGlobal, int idIndex, RESULT expValue ) {
    int callStackLastIndex = 0 ;
    
    TOKENTYPE idType ;
    
    if ( isIdGlobal ) {
      idType = gGlobalSymbolList[idIndex].symbolReturnType ;
      
      if ( idType == INT || idType == FLOAT ) {
        if ( expValue.valueType == INT || expValue.valueType == FLOAT ) {
          gGlobalSymbolList[idIndex].symbolValue.numValue = 
          gGlobalSymbolList[idIndex].symbolValue.numValue - expValue.numValue ;
          
          if ( idType == INT ) returnResult.valueType = INT ;
          else returnResult.valueType = FLOAT ;
          returnResult.numValue = gGlobalSymbolList[idIndex].symbolValue.numValue ;
        } 
      } 
    } 
    
    else {
      callStackLastIndex = gCallStack.size()-1 ;
      
      idType = gCallStack[callStackLastIndex].localList[idIndex].symbolReturnType ;
      
      if ( idType == INT || idType == FLOAT ) {
        if ( expValue.valueType == INT || expValue.valueType == FLOAT ) {
          gCallStack[callStackLastIndex].localList[idIndex].symbolValue.numValue = 
          gCallStack[callStackLastIndex].localList[idIndex].symbolValue.numValue - expValue.numValue ;
          
          if ( idType == INT ) returnResult.valueType = INT ;
          else returnResult.valueType = FLOAT ;
          returnResult.numValue = gCallStack[callStackLastIndex].localList[idIndex].symbolValue.numValue ;
        } 
      } 
    } 
  } 
  
  void PPMM_Array_Compute( RESULT &returnResult, TOKENTYPE operationValue, 
                           bool isIdGlobal, int idIndex, int arrayIndex ) {
    int arrayInfoEntry = 0;
    int callStackLastIndex = 0;
    ArrayInfo a ;
    
    if ( isIdGlobal ) {
      
      arrayInfoEntry = gGlobalSymbolList[idIndex].arrayEntry ;
      
      
      a.arrayTable = gArrayInfoTable[arrayInfoEntry].arrayTable ;
      a.arrayEntry = gArrayInfoTable[arrayInfoEntry].arrayEntry ;
    } 
    
    else {
      
      callStackLastIndex = gCallStack.size()-1;
      
      
      arrayInfoEntry = gCallStack[callStackLastIndex].localList[idIndex].arrayEntry ;
      
      
      a.arrayTable = gArrayInfoTable[arrayInfoEntry].arrayTable ;
      a.arrayEntry = gArrayInfoTable[arrayInfoEntry].arrayEntry ;
    } 
    
    
    if ( a.arrayTable == 1 ) {
      if ( operationValue == PP ) 
        gArrayIntTable[a.arrayEntry+arrayIndex] = gArrayIntTable[a.arrayEntry+arrayIndex] + 1 ;
      else 
        gArrayIntTable[a.arrayEntry+arrayIndex] = gArrayIntTable[a.arrayEntry+arrayIndex] - 1 ;
      returnResult.valueType = INT ;
      returnResult.numValue = gArrayIntTable[a.arrayEntry+arrayIndex] ;
    } 
    
    
    else if ( a.arrayTable == 2 ) {
      if ( operationValue == PP ) 
        gArrayFloatTable[a.arrayEntry+arrayIndex] = gArrayFloatTable[a.arrayEntry+arrayIndex] + 1 ;
      else 
        gArrayFloatTable[a.arrayEntry+arrayIndex] = gArrayFloatTable[a.arrayEntry+arrayIndex] - 1 ;
      returnResult.valueType = FLOAT ;
      returnResult.numValue = gArrayFloatTable[a.arrayEntry+arrayIndex] ;
    } 
  } 
  
  void PPMM_Compute( RESULT &returnResult, TOKENTYPE operationValue, bool isIdGlobal, int idIndex ) {
    int callStackLastIndex = 0;
    TOKENTYPE idType ;
    
    if ( isIdGlobal ) {
      idType = gGlobalSymbolList[idIndex].symbolReturnType ;
      
      if ( idType == INT || idType == FLOAT ) {
        if ( operationValue == PP ) gGlobalSymbolList[idIndex].symbolValue.numValue++ ;
        else gGlobalSymbolList[idIndex].symbolValue.numValue-- ;
          
        if ( idType == INT ) returnResult.valueType = INT ;
        else returnResult.valueType = FLOAT ;
        returnResult.numValue = gGlobalSymbolList[idIndex].symbolValue.numValue ;
      } 
    } 
    
    else {
      callStackLastIndex = gCallStack.size()-1 ;
      idType = gCallStack[callStackLastIndex].localList[idIndex].symbolReturnType ;
      
      if ( idType == INT || idType == FLOAT ) {
        if ( operationValue == PP ) 
          gCallStack[callStackLastIndex].localList[idIndex].symbolValue.numValue++ ;
        else 
          gCallStack[callStackLastIndex].localList[idIndex].symbolValue.numValue-- ;
          
        if ( idType == INT ) returnResult.valueType = INT ;
        else returnResult.valueType = FLOAT ;
        returnResult.numValue = gCallStack[callStackLastIndex].localList[idIndex].symbolValue.numValue ;
      } 
    } 
    
  } 
  
  bool Rest_of_Identifier_started_basic_exp( int &nextTokenIndex, RESULT &returnResult, RESULT preValue ) {
    TOKENTYPE tokentype ;
    TOKENTYPE operationValue ;
    RESULT expValue ;
    RESULT basicExpValue ;
    RESULT rrValue ;
    vector<RESULT> actualParListValue ;
    
    RESULT r ;
    RESULT r2 ;
    bool isIdGlobal = false ;
    int idIndex = -1 ;
    int arrayIndex = -1 ;
    
    
    if ( !mIsCompute ) {
      
      PeekToken( tokentype ) ;
      
      if ( tokentype == LBK ) {
        
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        if ( Expression( nextTokenIndex, expValue ) ) {
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
          
          
          if ( tokentype == RBK ) {
            
            PeekToken( tokentype ) ;
            
            if ( Assignment_operator( tokentype ) ) {
              
              GetToken( tokentype ) ;
              gAllTokenStringList.push_back( gToken ) ;
              
              
              if ( Basic_expression( nextTokenIndex, basicExpValue ) ) return true ;
              else return false ; 
            } 
            
            else if ( tokentype == PP ||  tokentype == MM ) {
              
              
              GetToken( tokentype ) ;
              gAllTokenStringList.push_back( gToken ) ;
              
              
              if ( Romce_and_romloe( nextTokenIndex, rrValue, preValue ) ) return true ;
              else return false ; 
            } 
            
            else {
              
              if ( Romce_and_romloe( nextTokenIndex, rrValue, preValue ) ) return true ;
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
        
        
        if ( Basic_expression( nextTokenIndex, basicExpValue ) ) return true ;
        else return false ; 
      } 
      
      else if ( tokentype == PP || tokentype == MM ) {
        
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        if ( Romce_and_romloe( nextTokenIndex, rrValue, preValue ) ) return true ;
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
          
          
          if ( !Actual_parameter_list( nextTokenIndex, actualParListValue ) ) {
            return false ;
          } 
        } 
        
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        if ( tokentype == RP ) {
          
          if ( Romce_and_romloe( nextTokenIndex, rrValue, preValue ) ) return true ;
          else return false ; 
        } 
        
        
        else {
          
          ThrowException( tokentype ) ;
          return false ;
        } 
      } 
      
      
      else {
        
        if ( Romce_and_romloe( nextTokenIndex, rrValue, preValue ) ) return true ;
        else return false ; 
      } 
    } 
    
    
    else {
      
      
      
      if ( preValue.idName.compare( "cout" ) != 0 ) 
        ReturnNonFunctionIdLocation( isIdGlobal, idIndex, preValue.idName ) ;
      
      
      
      PassTwoPeekToken( nextTokenIndex, tokentype ) ;
    
      
      if ( tokentype == LBK ) {
        
        
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++ ;
        
        
        
        Expression( nextTokenIndex, expValue ) ;
        
        
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++ ;
        
        
        arrayIndex = expValue.numValue ;
        
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
        
        
        if ( Assignment_operator( tokentype ) ) {         
          
          operationValue = tokentype ;
           
          
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex++ ;
          
          
          
          Basic_expression( nextTokenIndex, basicExpValue ) ;
          
          
          
          
          if ( mDoExp ) {
            
            if ( operationValue == ASSIGN ) {
              
              
              AssignArrayValue( isIdGlobal, idIndex, arrayIndex, basicExpValue ) ;
              
              
              returnResult = basicExpValue ;
    
            } 
            
            
            else if ( operationValue == TE ) {
              TE_Array_Compute( r, isIdGlobal, idIndex, arrayIndex, basicExpValue ) ;
              
              
              returnResult = r ;
            } 
            
            
            else if ( operationValue == DE ) {
              DE_Array_Compute( r, isIdGlobal, idIndex, arrayIndex, basicExpValue ) ;
              
              
              returnResult = r ;
            } 
            
            
            else if ( operationValue == RE ) {
              RE_Array_Compute( r, isIdGlobal, idIndex, arrayIndex, basicExpValue ) ;
              
              
              returnResult = r ;
            } 
            
            
            else if ( operationValue == PE ) {
              PE_Array_Compute( r, isIdGlobal, idIndex, arrayIndex, basicExpValue ) ;
              
              
              returnResult = r ;
            } 
            
            
            else if ( operationValue == ME ) {
              ME_Array_Compute( r, isIdGlobal, idIndex, arrayIndex, basicExpValue ) ;
              
              
              returnResult = r ;
            } 
          } 
          
          return true ; 
        } 
        
        
        else if ( tokentype == PP ||  tokentype == MM ) {
          
          operationValue = tokentype ;
           
          
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex++ ;
          
          
          ReturnIdArrayValue( r, isIdGlobal, idIndex, arrayIndex ) ;
          
          
          if ( mDoExp ) {
            
            
            PPMM_Array_Compute( r2, operationValue, isIdGlobal, idIndex, arrayIndex ) ;
          } 

          
          
          
          Romce_and_romloe( nextTokenIndex, rrValue, r ) ;
          
          
          returnResult = rrValue ; 
           
          return true ; 
        } 
        
        else {
          
          
          ReturnIdArrayValue( r, isIdGlobal, idIndex, arrayIndex ) ;
          
          
          
          Romce_and_romloe( nextTokenIndex, rrValue, r ) ;
          
          
          returnResult = rrValue ;
          
          return true ;
        } 
      } 
      
      
      else if ( Assignment_operator( tokentype ) ) {
        
        
        operationValue = tokentype ;
         
        
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++ ;
        
        
        
        Basic_expression( nextTokenIndex, basicExpValue ) ; 

        
        
        
        if ( mDoExp ) {
          
          if ( operationValue == ASSIGN ) {
            
            AssignValue( isIdGlobal, idIndex, basicExpValue ) ;
            
            
            returnResult = basicExpValue ;
          } 
          
          
          else if ( operationValue == TE ) {
            TE_Compute( r, isIdGlobal, idIndex, basicExpValue ) ;
            
            
            returnResult = r ;
          } 
          
          
          else if ( operationValue == DE ) {
            DE_Compute( r, isIdGlobal, idIndex, basicExpValue ) ;
            
            
            returnResult = r ;
          } 
          
          
          else if ( operationValue == RE ) {
            RE_Compute( r, isIdGlobal, idIndex, basicExpValue ) ;
            
            
            returnResult = r ;
          } 
          
          
          else if ( operationValue == PE ) {
            PE_Compute( r, isIdGlobal, idIndex, basicExpValue ) ;
            
            
            returnResult = r ;
          } 
          
          
          else if ( operationValue == ME ) {
            ME_Compute( r, isIdGlobal, idIndex, basicExpValue ) ;
            
            
            returnResult = r ;
          } 
        } 
        
        return true ; 
      } 
      
      else if ( tokentype == PP || tokentype == MM ) {
        
        operationValue = tokentype ;
        
        
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++ ;
        
        
        ReturnIdValue( r, isIdGlobal, idIndex ) ;
        
        
        if ( mDoExp ) {
          
          PPMM_Compute( r2, operationValue, isIdGlobal, idIndex ) ;
        } 

        
        
        Romce_and_romloe( nextTokenIndex, rrValue, r ) ;
 
        
        returnResult = rrValue ;
        
        return true ;
      } 
      
      
      else if ( tokentype == LP ) {
        /*
        
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++ ;
        
        
        PeekToken( tokentype ) ;
        
        
        if ( tokentype == IDENTIFIER || tokentype == PP || tokentype == MM 
             || Sign( tokentype ) || tokentype == CONSTANT_INT
             || tokentype == CONSTANT_FLOAT || tokentype == CONSTANT_CHAR 
             || tokentype == CONSTANT_STRING || tokentype == CONSTANT_BOOL 
             || tokentype == LP ) {
          
          
          if ( !Actual_parameter_list( nextTokenIndex, actualParListValue ) ) {
            return false ;
          } 
        } 
        
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
        
        
        if ( tokentype == RP ) {
          
          if ( Romce_and_romloe( nextTokenIndex, rrValue ) ) return true ;
          else return false ; 
        } 
        
        
        else {
          
          ThrowException( tokentype ) ;
          return false ;
        } 
        */
      } 
      
      
      else {
        
        
        
        if ( preValue.idName.compare( "cout" ) != 0 ) ReturnIdValue( r, isIdGlobal, idIndex ) ;
        
        else {
          r.valueType = IDENTIFIER ;
          r.idName = "cout" ;
        } 
        
        
        
        Romce_and_romloe( nextTokenIndex, rrValue, r ) ;
        
        
        returnResult = rrValue ;
        
        return true ;
      } 
    } 
    
    return false ;
  } 
  
  
  
  bool Rest_of_PPMM_Identifier_started_basic_exp( int &nextTokenIndex, RESULT &returnResult, 
                                                  RESULT preValue, TOKENTYPE operationValue ) {
    TOKENTYPE tokentype ;
    RESULT expValue ;
    RESULT rrValue ;
    RESULT r ;
    
    bool isIdGlobal = false ;
    int idIndex = -1 ;
    int arrayIndex = -1 ; 
    
    
    if ( !mIsCompute ) {
      
      PeekToken( tokentype ) ;
      
      if ( tokentype == LBK ) {
        
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ; 
        
        
        if ( Expression( nextTokenIndex, expValue ) ) {
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ; 
          
          
          if ( tokentype != RBK ) {
            
            ThrowException( tokentype ) ;
            return false ;
          } 
        } 
        
        else return false ; 
      } 
      
      
      if ( Romce_and_romloe( nextTokenIndex, rrValue, preValue ) ) return true ;
      else return false ; 
    } 
    
    
    else {
      
      
      ReturnNonFunctionIdLocation( isIdGlobal, idIndex, preValue.idName ) ;
      
      
      PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      
      if ( tokentype == LBK ) {
        
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++ ;
        
        
        
        Expression( nextTokenIndex, expValue ) ;
        
        
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++ ;
          
        arrayIndex = expValue.numValue ;
        
        
        if ( mDoExp ) {
          
          
          PPMM_Array_Compute( r, operationValue, isIdGlobal, idIndex, arrayIndex ) ;
        } 
                
      } 
      
      
      else {
        
        if ( mDoExp ) {
          
          
          PPMM_Compute( r, operationValue, isIdGlobal, idIndex )  ;
        } 
      } 
      
      
      
      Romce_and_romloe( nextTokenIndex, rrValue, r ) ;
      
      
      returnResult = rrValue ;
      
      return true ;
    } 
    
    return false ;
  } 
  
  bool Sign( TOKENTYPE tokentype ) {
    if ( tokentype == PLUS || tokentype == MINUS || tokentype == NOT ) return true ;
    return false ;
  } 
  
  
  
  bool Actual_parameter_list( int &nextTokenIndex,  vector<RESULT> &returnResultList ) {
    TOKENTYPE tokentype ;
    RESULT basicExpValue ;
  
    
    if ( Basic_expression( nextTokenIndex, basicExpValue ) ) {
      
      
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
        
        
        if ( Basic_expression( nextTokenIndex, basicExpValue ) ) {
          
          
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
  
  bool IsValueTrue( RESULT conditionValue ) {
    if ( conditionValue.valueType == INT || conditionValue.valueType == FLOAT ) {
      if ( conditionValue.numValue >= 1 ) return true ;
    } 
    
    else if ( conditionValue.valueType == BOOL ) {
      if ( conditionValue.boolValue ) return true ;
    } 
    
    return false ;
  } 
  
  
  
  bool Romce_and_romloe( int &nextTokenIndex,  RESULT &returnResult, RESULT preValue ) {
    TOKENTYPE tokentype ;
    RESULT restLogicOrExpValue ;
    RESULT basicExpValue1 ;
    RESULT basicExpValue2 ;
    RESULT r ;
    
    
    if ( !mIsCompute ) {
      
      if ( Rest_of_maybe_logical_OR_exp( nextTokenIndex, restLogicOrExpValue, preValue ) ) {
        
        PeekToken( tokentype ) ;
        
        
        if ( tokentype == QMARK ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;

          
          if ( Basic_expression( nextTokenIndex, basicExpValue1 ) ) {
            
            
            GetToken( tokentype ) ;
            gAllTokenStringList.push_back( gToken ) ;
            
            
            if ( tokentype == COLON ) {
              
              if ( Basic_expression( nextTokenIndex, basicExpValue2 ) ) return true ;
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
    } 
    
    
    else {
      
      
      
      Rest_of_maybe_logical_OR_exp( nextTokenIndex, restLogicOrExpValue, preValue ) ;
      
      
      PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      
      
      
      returnResult = restLogicOrExpValue ;
      
      
      if ( tokentype == QMARK ) {
        
        
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++ ;
        
        
        if ( IsValueTrue( restLogicOrExpValue ) ) {
          
          
          Basic_expression( nextTokenIndex, basicExpValue1 ) ;
          
          
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex++ ;
          
          
          mDoExp = false ;
          mCoutExpFalse++ ;
          Basic_expression( nextTokenIndex, basicExpValue2 ) ;
          
          mCoutExpFalse-- ;
          
          
          
          if ( mCoutExpFalse == 0 ) {
            mDoExp = true ;
          } 
          
          
          returnResult = basicExpValue1 ;
        } 
        
        
        else {
          
          
          SkipTrueStatementInRomce( nextTokenIndex ) ;
          
          
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex++ ;
          
          
          Basic_expression( nextTokenIndex, basicExpValue2 ) ;
          
          
          returnResult = basicExpValue2 ;
        } 
      } 
      
      return true ;
    } 
       
    return false ; 
  } 
  
  void OR_Compute( RESULT &returnResult, RESULT tmp1, RESULT tmp2 ) {
    
    
    if ( tmp1.valueType == INT || tmp1.valueType == FLOAT ) {
      
      if ( tmp1.numValue >= 1 ) {
        returnResult.boolValue = true ;
      } 
      
      
      else {
        
        if ( tmp2.valueType == INT || tmp2.valueType == FLOAT ) {
          
          if ( tmp2.numValue >= 1 ) returnResult.boolValue = true ; 
          else returnResult.boolValue = false ; 
        } 
        
        
        else if ( tmp2.valueType == BOOL ) {
          
          if ( tmp2.boolValue ) returnResult.boolValue = true ;
          else returnResult.boolValue = false ; 
        } 
        
        
        else {
          returnResult.boolValue = false ;
        } 
      } 
    } 
    
    
    else if ( tmp1.valueType == BOOL ) {
      
      if ( tmp1.boolValue ) returnResult.boolValue = true ;
      
      
      else {
        
        if ( tmp2.valueType == INT || tmp2.valueType == FLOAT ) {
          
          if ( tmp2.numValue >= 1 ) returnResult.boolValue = true ;
          else returnResult.boolValue = false ; 
        } 
        
        
        else if ( tmp2.valueType == BOOL ) {
          
          if ( tmp2.boolValue ) returnResult.boolValue = true ;
          else returnResult.boolValue = false ; 
        } 
        
        
        else {
          returnResult.boolValue = false ;
        } 
      } 
    } 
    
    
    else {
      returnResult.boolValue = false ;
    } 
    
    returnResult.valueType = BOOL ;
  } 
  
  void AND_Compute( RESULT &returnResult, RESULT tmp1, RESULT tmp2 ) {
    
    
    if ( tmp1.valueType == INT || tmp1.valueType == FLOAT ) {
      
      
      if ( tmp2.valueType == INT || tmp2.valueType == FLOAT ) {
        if ( tmp1.numValue >= 1 && tmp2.numValue >= 1 )
          returnResult.boolValue = true ; 
        else  
          returnResult.boolValue = false ;
      } 
      
      
      else if ( tmp2.valueType == BOOL ) {
        if ( tmp1.numValue >= 1 && tmp2.boolValue )
          returnResult.boolValue = true ; 
        else  
          returnResult.boolValue = false ;
      } 
      
      
      else {
        returnResult.boolValue = false ;
      } 
    } 
    
    
    else if ( tmp1.valueType == BOOL ) {
      
      if ( tmp2.valueType == INT || tmp2.valueType == FLOAT ) {
        if ( tmp1.boolValue && tmp2.numValue >= 1 )
          returnResult.boolValue = true ; 
        else  
          returnResult.boolValue = false ;
      } 
      
      
      else if ( tmp2.valueType == BOOL ) {
        if ( tmp1.boolValue && tmp2.boolValue )
          returnResult.boolValue = true ; 
        else  
          returnResult.boolValue = false ;
      } 
      
      
      else {
        returnResult.boolValue = false ;
      } 
    } 
    
    
    else {
      returnResult.boolValue = false ;
    } 
    
    returnResult.valueType = BOOL ;
  } 
  
  bool Rest_of_maybe_logical_OR_exp( int &nextTokenIndex,  RESULT &returnResult, RESULT preValue ) {
    TOKENTYPE tokentype ;
    RESULT restLogicAndExpValue ;
    RESULT logicAndExpValue ;
    RESULT r ;
    
    
    if ( Rest_of_maybe_logical_AND_exp( nextTokenIndex, restLogicAndExpValue, preValue ) ) {
      
      
      
      if ( mIsCompute ) r = restLogicAndExpValue ;
      
      
      
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
        
        
        if ( Maybe_logical_AND_exp( nextTokenIndex, logicAndExpValue ) ) {
          
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            
            if ( mIsCompute ) {
              OR_Compute( r, r, logicAndExpValue ) ;
            } 
            
            
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      
      
      if ( mIsCompute ) returnResult = r ;
      
      
      
      return true ;
    } 
    
    return false ; 
  } 
   
  bool Maybe_logical_AND_exp( int &nextTokenIndex, RESULT &returnResult ) {
    TOKENTYPE tokentype ;
    RESULT bitOrExpValue1 ;
    RESULT bitOrExpValue2 ;
    RESULT r ;
    
    
    if ( Maybe_bit_OR_exp( nextTokenIndex, bitOrExpValue1 ) ) {
      
      
      
      if ( mIsCompute ) r = bitOrExpValue1 ;
      
      
      
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
      
        
        if ( Maybe_bit_OR_exp( nextTokenIndex, bitOrExpValue2 ) ) {
          
          if ( !mIsCompute  ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            
            if ( mIsCompute ) {
              AND_Compute( r, r, bitOrExpValue2 ) ;
            } 
            
            
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      
      
      if ( mIsCompute ) returnResult = r ;
      
       
      return true ;
    } 
    
    return false ; 
  } 
  
  bool Rest_of_maybe_logical_AND_exp( int &nextTokenIndex,  RESULT &returnResult, RESULT preValue ) {
    TOKENTYPE tokentype ;
    RESULT restBitOrExpValue ;
    RESULT bitOrExpValue ;
    RESULT r ;
    
    
    if ( Rest_of_maybe_bit_OR_exp( nextTokenIndex, restBitOrExpValue, preValue ) ) {
      
      
      
      if ( mIsCompute ) r = restBitOrExpValue ;
      
       
      
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
        
        
        if ( Maybe_bit_OR_exp( nextTokenIndex, bitOrExpValue ) ) {
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            
            if ( mIsCompute ) {
              AND_Compute( r, r, bitOrExpValue ) ;
            } 
            
             
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      
      
      if ( mIsCompute ) returnResult = r ;
      
       
      return true ;
    } 
    
    return false ; 
  } 
  
  
  bool Maybe_bit_OR_exp( int &nextTokenIndex,  RESULT &returnResult ) {
    TOKENTYPE tokentype ;
    RESULT bitOrExpValue1 ;
    RESULT bitOrExpValue2 ;
    
    
    if ( Maybe_bit_ex_OR_exp( nextTokenIndex, bitOrExpValue1 ) ) {
      
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
        
        
        if ( Maybe_bit_ex_OR_exp( nextTokenIndex, bitOrExpValue2 ) ) {
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      
      
      if ( mIsCompute ) returnResult = bitOrExpValue1 ;
      
      
      return true ;
    } 
    
    return false ; 
  } 
  
  
  bool Rest_of_maybe_bit_OR_exp( int &nextTokenIndex,  RESULT &returnResult, RESULT preValue ) {
    TOKENTYPE tokentype ;
    RESULT restBitOrExpValue ;
    RESULT bitOrExpValue ;
    
    
    if ( Rest_of_maybe_bit_ex_OR_exp( nextTokenIndex, restBitOrExpValue, preValue ) ) {
      
      
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
        
        
        if ( Maybe_bit_ex_OR_exp( nextTokenIndex, bitOrExpValue ) ) {
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      
      
      if ( mIsCompute ) returnResult = restBitOrExpValue ;
      
      
      return true ;
    } 
    
    return false ; 
  } 
  
  
  bool Maybe_bit_ex_OR_exp( int &nextTokenIndex,  RESULT &returnResult ) {
    TOKENTYPE tokentype ;
    RESULT bitAndExpValue1 ;
    RESULT bitAndExpValue2 ;
    
    
    if ( Maybe_bit_AND_exp( nextTokenIndex, bitAndExpValue1 ) ) {
      
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
        
        
        if ( Maybe_bit_AND_exp( nextTokenIndex, bitAndExpValue2 ) ) {
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      
      
      if ( mIsCompute ) returnResult = bitAndExpValue1 ;
      
      
      return true ;
    } 
    
    return false ; 
  } 
  
  
  bool Rest_of_maybe_bit_ex_OR_exp( int &nextTokenIndex,  RESULT &returnResult, RESULT preValue ) {
    TOKENTYPE tokentype ;
    RESULT restBitAndExpValue ;
    RESULT bitAndExpValue ;
    
    
    if ( Rest_of_maybe_bit_AND_exp( nextTokenIndex, restBitAndExpValue, preValue ) ) {
      
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
        
        
        if ( Maybe_bit_AND_exp( nextTokenIndex, bitAndExpValue ) ) {
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      
      
      if ( mIsCompute ) returnResult = restBitAndExpValue ;
      
      
      return true ;
    } 

    return false ; 
  } 
  
  
  bool Maybe_bit_AND_exp( int &nextTokenIndex,  RESULT &returnResult ) {
    TOKENTYPE tokentype ;
    RESULT equalityExpValue1 ;
    RESULT equalityExpValue2 ;
    
    
    if ( Maybe_equality_exp( nextTokenIndex, equalityExpValue1 ) ) {
      
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
        
        
        if ( Maybe_equality_exp( nextTokenIndex, equalityExpValue2 ) ) {
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      
      
      if ( mIsCompute ) returnResult = equalityExpValue1 ;
      
      
      
      return true ;
    } 
  
    return false ; 
  } 
  
  
  bool Rest_of_maybe_bit_AND_exp( int &nextTokenIndex,  RESULT &returnResult, RESULT preValue ) {
    TOKENTYPE tokentype ;
    RESULT restEqualityExpValue ;
    RESULT equalityExpValue ;
    
    
    if ( Rest_of_maybe_equality_exp( nextTokenIndex, restEqualityExpValue, preValue ) ) {
      
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
        
        
        if ( Maybe_equality_exp( nextTokenIndex, equalityExpValue ) ) {
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      
      
      if ( mIsCompute ) returnResult = restEqualityExpValue ;
      
      
      return true ;
    } 
    
    return false ; 
  } 
  
  
  void EQ_Compute( RESULT &returnResult, RESULT tmp1, RESULT tmp2 ) {
    
    if ( tmp1.valueType == INT || tmp1.valueType == FLOAT ) {
      if ( tmp2.valueType == INT || tmp2.valueType == FLOAT ) {
        if ( tmp1.numValue == tmp2.numValue ) returnResult.boolValue = true ;
        else returnResult.boolValue = false ;
      } 
      
      else returnResult.boolValue = false ;
    } 
    
    else if ( tmp1.valueType == CHAR ) {
      if ( tmp2.valueType == CHAR ) {
        if ( tmp1.charValue == tmp2.charValue ) returnResult.boolValue = true ;
        else returnResult.boolValue = false ;
      } 
      
      else returnResult.boolValue = false ;
    } 
    
    else if ( tmp1.valueType == BOOL ) {
      if ( tmp2.valueType == BOOL ) {
        if ( tmp1.boolValue == tmp2.boolValue ) returnResult.boolValue = true ;
        else returnResult.boolValue = false ;
      } 
      
      else returnResult.boolValue = false ;
    } 
    
    else if ( tmp1.valueType == STRING ) {
      if ( tmp2.valueType == STRING ) {
        if ( tmp1.stringValue.compare( tmp2.stringValue ) == 0 ) 
          returnResult.boolValue = true ;
        else returnResult.boolValue = false ;
      } 
      
      else returnResult.boolValue = false ;
    } 
    
    returnResult.valueType = BOOL ;
    
  } 
  
  void NEQ_Compute( RESULT &returnResult, RESULT tmp1, RESULT tmp2 ) {
    
    if ( tmp1.valueType == INT || tmp1.valueType == FLOAT ) {
      if ( tmp2.valueType == INT || tmp2.valueType == FLOAT ) {
        if ( tmp1.numValue != tmp2.numValue ) returnResult.boolValue = true ;
        else returnResult.boolValue = false ;
      } 
      
      else returnResult.boolValue = false ;
    } 
    
    else if ( tmp1.valueType == CHAR ) {
      if ( tmp2.valueType == CHAR ) {
        if ( tmp1.charValue != tmp2.charValue ) returnResult.boolValue = true ;
        else returnResult.boolValue = false ;
      } 
      
      else returnResult.boolValue = false ;
    } 
    
    else if ( tmp1.valueType == BOOL ) {
      if ( tmp2.valueType == BOOL ) {
        if ( tmp1.boolValue != tmp2.boolValue ) returnResult.boolValue = true ;
        else returnResult.boolValue = false ;
      } 
      
      else returnResult.boolValue = false ;
    } 
    
    else if ( tmp1.valueType == STRING ) {
      if ( tmp2.valueType == STRING ) {
        if ( tmp1.stringValue.compare( tmp2.stringValue ) != 0 ) 
          returnResult.boolValue = true ;
        else returnResult.boolValue = false ;
      } 
      
      else returnResult.boolValue = false ;
    } 
    
    returnResult.valueType = BOOL ;
    
  } 
  
  bool Maybe_equality_exp( int &nextTokenIndex,  RESULT &returnResult ) {
    TOKENTYPE tokentype ;
    TOKENTYPE operationValue ;
    RESULT relationalExpValue1 ;
    RESULT relationalExpValue2 ; 
    RESULT r ;
    
    
    if ( Maybe_relational_exp( nextTokenIndex, relationalExpValue1 ) ) {
      
      
      
      if ( mIsCompute ) r = relationalExpValue1 ; 
      
      
      
      if ( !mIsCompute ) {
        
        PeekToken( tokentype ) ;
      } 
      
      
      else {
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      } 
      
      while ( tokentype == EQ || tokentype == NEQ ) {
        
        operationValue = tokentype ;
        
        
        if ( !mIsCompute ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
        } 
        
        
        else {
          
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex = nextTokenIndex + 1 ;
        } 
        
        
        if ( Maybe_relational_exp( nextTokenIndex, relationalExpValue2 ) ) {
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            
            if ( operationValue == EQ ) EQ_Compute( r, r, relationalExpValue2 ) ;
            else NEQ_Compute( r, r, relationalExpValue2 ) ;
            
            
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 

      } 
      
      
      
      if ( mIsCompute ) returnResult = r ;
      
      return true ;
    } 
    
    return false ; 
  } 
  
  bool Rest_of_maybe_equality_exp( int &nextTokenIndex,  RESULT &returnResult, RESULT preValue ) {
    TOKENTYPE tokentype ;
    TOKENTYPE operationValue ;
    RESULT restRelationalExpValue ;
    RESULT relationalExpValue ;
    RESULT r ;
    
    
    if ( Rest_of_maybe_relational_exp( nextTokenIndex, restRelationalExpValue, preValue ) ) {
      
      
      if ( mIsCompute ) r = restRelationalExpValue ;
      
      
      
      if ( !mIsCompute ) {
        
        PeekToken( tokentype ) ;
      } 
      
      
      else {
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      } 
      
      while ( tokentype == EQ || tokentype == NEQ ) {
        
        
        operationValue = tokentype ;
         
        
        if ( !mIsCompute ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
        } 
        
        
        else {
          
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex = nextTokenIndex + 1 ;
        } 
        
        
        if ( Maybe_relational_exp( nextTokenIndex, relationalExpValue ) ) {
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            
            
            if ( operationValue == EQ ) EQ_Compute( r, r, relationalExpValue ) ;
            else NEQ_Compute( r, r, relationalExpValue ) ;
            
            
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      
      
      if ( mIsCompute ) returnResult = r ;
      
       
      return true ;
    } 
    
    return false ; 
  } 
  
  void GT_Compute( RESULT &returnResult, RESULT tmp1, RESULT tmp2 ) {
    
    if ( tmp1.valueType == INT || tmp1.valueType == FLOAT ) {
      if ( tmp2.valueType == INT || tmp2.valueType == FLOAT ) {
        if ( tmp1.numValue > tmp2.numValue ) returnResult.boolValue = true ;
        else returnResult.boolValue = false ;
      } 
      
      else returnResult.boolValue = false ;
    
    } 
    
    else if ( tmp1.valueType == CHAR ) {
      if ( tmp2.valueType == CHAR ) {
        if ( tmp1.charValue > tmp2.charValue ) returnResult.boolValue = true ;
        else returnResult.boolValue = false ;
      } 
      
      else returnResult.boolValue = false ;
    } 
    
    else if ( tmp1.valueType == BOOL ) {
      if ( tmp2.valueType == BOOL ) {
        if ( tmp1.boolValue > tmp2.boolValue ) returnResult.boolValue = true ;
        else returnResult.boolValue = false ;
      } 
      
      else returnResult.boolValue = false ;
    } 
    
    else if ( tmp1.valueType == STRING ) {
      if ( tmp2.valueType == STRING ) {
        if ( tmp1.stringValue > tmp2.stringValue ) returnResult.boolValue = true ;
        else returnResult.boolValue = false ;
      } 
      
      else returnResult.boolValue = false ;
    } 
    
    returnResult.valueType = BOOL ;
  } 
  
  void ST_Compute( RESULT &returnResult, RESULT tmp1, RESULT tmp2 ) {
    
    if ( tmp1.valueType == INT || tmp1.valueType == FLOAT ) {
      if ( tmp2.valueType == INT || tmp2.valueType == FLOAT ) {
        if ( tmp1.numValue < tmp2.numValue ) returnResult.boolValue = true ;
        else returnResult.boolValue = false ;
      } 
      
      else returnResult.boolValue = false ;
    
    } 
    
    else if ( tmp1.valueType == CHAR ) {
      if ( tmp2.valueType == CHAR ) {
        if ( tmp1.charValue < tmp2.charValue ) returnResult.boolValue = true ;
        else returnResult.boolValue = false ;
      } 
      
      else returnResult.boolValue = false ;
    } 
    
    else if ( tmp1.valueType == BOOL ) {
      if ( tmp2.valueType == BOOL ) {
        if ( tmp1.boolValue < tmp2.boolValue ) returnResult.boolValue = true ;
        else returnResult.boolValue = false ;
      } 
      
      else returnResult.boolValue = false ;
    } 
    
    else if ( tmp1.valueType == STRING ) {
      if ( tmp2.valueType == STRING ) {
        if ( tmp1.stringValue < tmp2.stringValue ) returnResult.boolValue = true ;
        else returnResult.boolValue = false ;
      } 
      
      else returnResult.boolValue = false ;
    } 
    
    returnResult.valueType = BOOL ;
  } 
  
  void LE_Compute( RESULT &returnResult, RESULT tmp1, RESULT tmp2 ) {
    
    if ( tmp1.valueType == INT || tmp1.valueType == FLOAT ) {
      if ( tmp2.valueType == INT || tmp2.valueType == FLOAT ) {
        if ( tmp1.numValue <= tmp2.numValue ) returnResult.boolValue = true ;
        else returnResult.boolValue = false ;
      } 
      
      else returnResult.boolValue = false ;
    
    } 
    
    else if ( tmp1.valueType == CHAR ) {
      if ( tmp2.valueType == CHAR ) {
        if ( tmp1.charValue <= tmp2.charValue ) returnResult.boolValue = true ;
        else returnResult.boolValue = false ;
      } 
      
      else returnResult.boolValue = false ;
    } 
    
    else if ( tmp1.valueType == BOOL ) {
      if ( tmp2.valueType == BOOL ) {
        if ( tmp1.boolValue <= tmp2.boolValue ) returnResult.boolValue = true ;
        else returnResult.boolValue = false ;
      } 
      
      else returnResult.boolValue = false ;
    } 
    
    else if ( tmp1.valueType == STRING ) {
      if ( tmp2.valueType == STRING ) {
        if ( tmp1.stringValue <= tmp2.stringValue ) returnResult.boolValue = true ;
        else returnResult.boolValue = false ;
      } 
      
      else returnResult.boolValue = false ;
    } 
    
    returnResult.valueType = BOOL ;
  } 
  
  void GE_Compute( RESULT &returnResult, RESULT tmp1, RESULT tmp2 ) {
    
    if ( tmp1.valueType == INT || tmp1.valueType == FLOAT ) {
      if ( tmp2.valueType == INT || tmp2.valueType == FLOAT ) {
        if ( tmp1.numValue >= tmp2.numValue ) returnResult.boolValue = true ;
        else returnResult.boolValue = false ;
      } 
      
      else returnResult.boolValue = false ;
    
    } 
    
    else if ( tmp1.valueType == CHAR ) {
      if ( tmp2.valueType == CHAR ) {
        if ( tmp1.charValue >= tmp2.charValue ) returnResult.boolValue = true ;
        else returnResult.boolValue = false ;
      } 
      
      else returnResult.boolValue = false ;
    } 
    
    else if ( tmp1.valueType == BOOL ) {
      if ( tmp2.valueType == BOOL ) {
        if ( tmp1.boolValue >= tmp2.boolValue ) returnResult.boolValue = true ;
        else returnResult.boolValue = false ;
      } 
      
      else returnResult.boolValue = false ;
    } 
    
    else if ( tmp1.valueType == STRING ) {
      if ( tmp2.valueType == STRING ) {
        if ( tmp1.stringValue >= tmp2.stringValue ) returnResult.boolValue = true ;
        else returnResult.boolValue = false ;
      } 
      
      else returnResult.boolValue = false ;
    } 
    
    returnResult.valueType = BOOL ;
  } 
  
  bool Maybe_relational_exp( int &nextTokenIndex, RESULT &returnResult ) {
    TOKENTYPE tokentype ;
    TOKENTYPE operationValue ;
    RESULT shiftExpValue1 ;
    RESULT shiftExpValue2 ;
    RESULT r ;
    
    
    if ( Maybe_shift_exp( nextTokenIndex, shiftExpValue1 ) ) {
      
      
      
      if ( mIsCompute ) r = shiftExpValue1 ; 
      
      
      
      if ( !mIsCompute ) {
        
        PeekToken( tokentype ) ;
      } 
      
      
      else {
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      } 
      
      while ( tokentype == GT || tokentype == ST || tokentype == LE || tokentype == GE ) {
        
        operationValue = tokentype ; 
        
        
        if ( !mIsCompute ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
        } 
        
        
        else {
          
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex = nextTokenIndex + 1 ;
        } 
        
        
        if ( Maybe_shift_exp( nextTokenIndex, shiftExpValue2 ) ) {
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            
            
            if ( operationValue == GT )
              GT_Compute( r, r, shiftExpValue2 ) ;
            else if ( operationValue == ST )
              ST_Compute( r, r, shiftExpValue2 ) ;
            else if ( operationValue == LE )
              LE_Compute( r, r, shiftExpValue2 ) ;
            else
              GE_Compute( r, r, shiftExpValue2 ) ;
            
            
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      
      
      if ( mIsCompute ) returnResult = r ;
      
       
      return true ;
    } 
     
    return false ; 
  } 
  
  bool Rest_of_maybe_relational_exp( int &nextTokenIndex,  RESULT &returnResult, RESULT preValue ) {
    TOKENTYPE tokentype ;
    TOKENTYPE operationValue ;
    RESULT restShiftExpValue ;
    RESULT shiftExpValue ;
    RESULT r ;
    
    
    if ( Rest_of_maybe_shift_exp( nextTokenIndex, restShiftExpValue, preValue ) ) {
      
      
      if ( mIsCompute ) r = restShiftExpValue ;
      
       
      
      if ( !mIsCompute ) {
        
        PeekToken( tokentype ) ;
      } 
      
      
      else {
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      } 
      
      while ( tokentype == GT || tokentype == ST || tokentype == LE || tokentype == GE ) {
        
        operationValue = tokentype ;
         
        
        if ( !mIsCompute ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
        } 
        
        
        else {
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex = nextTokenIndex + 1 ;
        } 
        
        
        if ( Maybe_shift_exp( nextTokenIndex, shiftExpValue ) ) {
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            
            if ( operationValue == GT )
              GT_Compute( r, r, shiftExpValue ) ;
            else if ( operationValue == ST )
              ST_Compute( r, r, shiftExpValue ) ;
            else if ( operationValue == LE )
              LE_Compute( r, r, shiftExpValue ) ;
            else
              GE_Compute( r, r, shiftExpValue ) ;
            
            
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      
      
      if ( mIsCompute ) returnResult = r ;
      
      
      return true ;
    } 
    
    return false ;  
  } 
  
  bool Maybe_shift_exp( int &nextTokenIndex, RESULT &returnResult ) {
    TOKENTYPE tokentype ;
    TOKENTYPE operationValue ;
    RESULT additiveExpValue1 ;
    RESULT additiveExpValue2 ;
    RESULT r ;
    
    int num1 = 0 ;
    int num2 = 0 ;
    
    
    if ( Maybe_additive_exp( nextTokenIndex, additiveExpValue1 ) ) {
      
      
      if ( mIsCompute ) r = additiveExpValue1 ;
      
       
      
      if ( !mIsCompute ) {
        
        PeekToken( tokentype ) ;
      } 
      
      
      else {
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      } 
      
      while ( tokentype == LS || tokentype == RS ) {
        
        
        operationValue = tokentype ; 
        
        
        if ( !mIsCompute ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
        } 
        
        
        else {
          
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex = nextTokenIndex + 1 ;
        } 
        
        
        if ( Maybe_additive_exp( nextTokenIndex, additiveExpValue2 ) ) {
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            
            
            if ( mIsCompute ) {
              
              num1 = r.numValue ;
              num2 = additiveExpValue2.numValue ;
              
              if ( operationValue == LS ) num1 = num1 << num2 ;
              else num1 = num1 >> num2 ;
              
              r.numValue = num1 ;
            } 
            
            
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      
      
      if ( mIsCompute ) returnResult = r ; 
      
      
      return true ;
    } 
    
    return false ; 
  } 
  
  bool Rest_of_maybe_shift_exp( int &nextTokenIndex,  RESULT &returnResult, RESULT preValue ) {
    TOKENTYPE tokentype ;
    TOKENTYPE operationValue ;
    RESULT restAdditiveExpValue ;
    RESULT additiveExpValue ;
    RESULT r ;
    
    int num1 = 0 ;
    int num2 = 0 ;
    
    
    if ( Rest_of_maybe_additive_exp( nextTokenIndex, restAdditiveExpValue, preValue ) ) {

      
      
      if ( mIsCompute ) r = restAdditiveExpValue ;
      
      
      
      if ( !mIsCompute ) {
        
        PeekToken( tokentype ) ;
      } 
      
      
      else {       
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      } 
      
      while ( tokentype == LS || tokentype == RS ) {
        
        
        operationValue = tokentype ;
         
        
        if ( !mIsCompute ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
        } 
        
        
        else {
          
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex = nextTokenIndex + 1 ;
        } 
        
        
        if ( Maybe_additive_exp( nextTokenIndex, additiveExpValue ) ) {
          
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {

            
            

            
            if ( operationValue == RS ) {
              num1 = r.numValue ;
              num2 = additiveExpValue.numValue ;
              
              num1 = num1 >> num2 ;
              r.numValue = num1 ;
            } 
            
            
            else {
              
              
              if ( r.valueType == IDENTIFIER ) {
                if ( r.idName.compare( "cout" ) == 0 ) {
                  
                  if ( mDoExp ) {
                    if ( additiveExpValue.valueType == INT ) {                   
                      cout << ( int ) additiveExpValue.numValue ;
                    } 
                    
                    else if ( additiveExpValue.valueType == FLOAT ) {
                      cout << additiveExpValue.numValue ;
                    } 
                    
                    else if ( additiveExpValue.valueType == CHAR ) {
                      cout << additiveExpValue.charValue ;
                    } 
                    
                    else if ( additiveExpValue.valueType == BOOL ) {
                      if ( additiveExpValue.boolValue ) cout << "true" ;
                      else cout << "false" ;
                    } 
                    
                    else if ( additiveExpValue.valueType == STRING ) {
                      cout << additiveExpValue.stringValue ;
                    } 
                  } 
                } 
              } 
              
              
              else {
                num1 = r.numValue ;
                num2 = additiveExpValue.numValue ;
                
                num1 = num1 << num2 ;
                r.numValue = num1 ;
              } 
            } 

            
            
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      
      
      if ( mIsCompute ) {
        
        
        if ( r.valueType == IDENTIFIER ) returnResult = additiveExpValue ;
        else returnResult = r ; 
      } 
      
      
      return true ;
    } 
  
    return false ; 
  } 
  
  void Plus_Compute( RESULT &returnResult, RESULT tmp1, RESULT tmp2 ) {
    int num = 0 ; 
    
    if ( tmp1.valueType == INT ) {
      if ( tmp2.valueType == INT ) {
        returnResult.valueType = INT ;
        returnResult.numValue = tmp1.numValue + tmp2.numValue ; 
      } 
      
      if ( tmp2.valueType == FLOAT ) {
        returnResult.valueType = FLOAT ;
        returnResult.numValue = tmp1.numValue + tmp2.numValue ;
      } 
      
      else if ( tmp2.valueType == STRING ) {
        returnResult.valueType = STRING ;
        num = tmp1.numValue ;
        returnResult.stringValue = IntToString( num ) + tmp2.stringValue ;
      } 
    } 
    
    else if ( tmp1.valueType == FLOAT ) {
      if ( tmp2.valueType == INT ) {
        returnResult.valueType = FLOAT ;
        returnResult.numValue = tmp1.numValue + tmp2.numValue ;
      } 
      
      if ( tmp2.valueType == FLOAT ) {
        returnResult.valueType = FLOAT ;
        returnResult.numValue = tmp1.numValue + tmp2.numValue ;
      } 
      
      else if ( tmp2.valueType == STRING ) {
        returnResult.valueType = STRING ;
        returnResult.stringValue = DoubleToString( tmp1.numValue ) + tmp2.stringValue ;
      } 
    } 
    
    else if ( tmp1.valueType == CHAR ) {
      if ( tmp2.valueType == STRING ) {
        returnResult.valueType = STRING ;
        returnResult.stringValue = tmp1.charValue + tmp2.stringValue ;
      } 
    } 
    
    else if ( tmp1.valueType == BOOL ) {      
      if ( tmp2.valueType == STRING ) {
        returnResult.valueType = STRING ;
        if ( tmp1.boolValue )
          returnResult.stringValue = "true" + tmp2.stringValue ;
        else 
          returnResult.stringValue = "false" + tmp2.stringValue ;
      } 
    } 
    
    else if ( tmp1.valueType == STRING ) {
      returnResult.valueType = STRING ;
      
      if ( tmp2.valueType == INT ) {
        num = tmp2.numValue ;
        returnResult.stringValue = tmp1.stringValue + IntToString( num ) ;
      } 
      
      else if ( tmp2.valueType == FLOAT ) {
        returnResult.stringValue = tmp1.stringValue + DoubleToString( tmp2.numValue ) ;
      } 
      
      else if ( tmp2.valueType == CHAR ) {
        returnResult.stringValue = tmp1.stringValue + tmp2.charValue ;
      } 
      
      else if ( tmp2.valueType == BOOL ) {
        if ( tmp2.boolValue ) 
          returnResult.stringValue = tmp1.stringValue + "true" ;
        else
          returnResult.stringValue = tmp1.stringValue + "false" ;
      } 
      
      else if ( tmp2.valueType == STRING ) {
        returnResult.stringValue = tmp1.stringValue + tmp2.stringValue ;
      } 
    } 
  } 
  
  void Minus_Compute( RESULT &returnResult, RESULT tmp1, RESULT tmp2 ) {
    if ( tmp1.valueType == INT || tmp1.valueType == FLOAT ) {
      if ( tmp2.valueType == INT || tmp2.valueType == FLOAT ) {
        if ( tmp1.valueType == INT && tmp2.valueType == INT )
          returnResult.valueType = INT ;
        else
          returnResult.valueType = FLOAT ;
        
        returnResult.numValue = tmp1.numValue - tmp2.numValue ;
      } 
    } 
  } 
  
  
  bool Maybe_additive_exp( int &nextTokenIndex, RESULT &returnResult ) {
    TOKENTYPE tokentype ;
    TOKENTYPE operationValue ;
    RESULT multExpValue1 ;
    RESULT multExpValue2 ;
    RESULT r ;
  
    
    
    if ( Maybe_mult_exp( nextTokenIndex, multExpValue1 ) ) {
      
      
      
      if ( mIsCompute ) {
        r = multExpValue1 ;
      } 
      
      
      
      if ( !mIsCompute ) {
        
        PeekToken( tokentype ) ;
      } 
      
      
      else {
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      } 
      
      while ( tokentype == PLUS || tokentype == MINUS ) {
        
        
        operationValue = tokentype ;
        
         
        
        if ( !mIsCompute ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
        } 
        
        
        else {
          
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex = nextTokenIndex + 1 ;
        } 
        
        
        if ( Maybe_mult_exp( nextTokenIndex, multExpValue2 ) ) {
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            
            if ( operationValue == PLUS ) 
              Plus_Compute( r, r, multExpValue2 ) ;
            else
              Minus_Compute( r, r, multExpValue2 ) ;
            
            
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      
      
      if ( mIsCompute ) {
        returnResult = r ; 
      } 
      
      
      return true ;
    } 
    
    return false ; 
  } 
  
  bool Rest_of_maybe_additive_exp( int &nextTokenIndex,  RESULT &returnResult, RESULT preValue ) {
    TOKENTYPE tokentype ;
    TOKENTYPE operationValue ;
    RESULT restMultExpValue ;
    RESULT multExpValue ;
    RESULT r ;
    RETURNID returnID ;
    
    
    returnID.changeValue = false ;
    returnID.isArray = false ; 
    
    
    if ( Rest_of_maybe_mult_exp( nextTokenIndex, restMultExpValue, preValue, returnID ) ) {
      
      
      if ( mIsCompute ) r = restMultExpValue ; 
      
      
      
      if ( !mIsCompute ) {
        
        PeekToken( tokentype ) ;
      } 
      
      
      else {
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      } 
      
      while ( tokentype == PLUS || tokentype == MINUS ) {
        
        
        operationValue = tokentype ;
         
        
        if ( !mIsCompute ) {
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
        } 
        
        
        else {
          
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex = nextTokenIndex + 1 ;
        } 
        
        
        if ( Maybe_mult_exp( nextTokenIndex, multExpValue ) ) {
          
          if ( !mIsCompute ) {
            
            PeekToken( tokentype ) ;
          } 
          
          
          else {
            
            
            if ( operationValue == PLUS ) 
              Plus_Compute( r, r, multExpValue ) ;
            else 
              Minus_Compute( r, r, multExpValue ) ;
            
            
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
          } 
        } 
        
        else return false ; 
      } 
      
      
      
      if ( mIsCompute ) {
        returnResult = r ;
      } 
      
      
      return true ;
    } 

    return false ; 
  } 
   
  bool Maybe_mult_exp( int &nextTokenIndex, RESULT &returnResult ) {
    TOKENTYPE tokentype ;
    RESULT unaryExpValue ;
    RESULT restMultExpValue ;
    RETURNID returnID ;
    
    
    if ( Unary_exp( nextTokenIndex, unaryExpValue, returnID ) ) {
      
      
      if ( Rest_of_maybe_mult_exp( nextTokenIndex, restMultExpValue, unaryExpValue, returnID ) ) {
        
        
        
        if ( mIsCompute ) {
          returnResult = restMultExpValue ;
        } 
        
        
        return true ; 
      } 
        
      else return false ; 
    } 
    
    return false ; 
  } 
  
  void MUL_Compute( RESULT &returnResult, RESULT tmp1, RESULT tmp2 ) {
    
    if ( tmp1.valueType == INT || tmp1.valueType == FLOAT ) {
      if ( tmp2.valueType == INT || tmp2.valueType == FLOAT ) {
        if ( tmp1.valueType == INT && tmp2.valueType == INT )
          returnResult.valueType = INT ;
        else
          returnResult.valueType = FLOAT ;
        
        returnResult.numValue = tmp1.numValue * tmp2.numValue ;
      } 
    } 
  } 
  
  void DIV_Compute( RESULT &returnResult, RESULT tmp1, RESULT tmp2 ) {
    int num1 = 0 ;
    int num2 = 0 ;
    
    if ( tmp1.valueType == INT || tmp1.valueType == FLOAT ) {
      if ( tmp2.valueType == INT || tmp2.valueType == FLOAT ) {
        if ( tmp1.valueType == INT && tmp2.valueType == INT ) {
          returnResult.valueType = INT ;
          num1 = tmp1.numValue ;
          num2 = tmp2.numValue ;
          returnResult.numValue = num1 / num2 ;
        } 
          
        else {
          returnResult.valueType = FLOAT ;
          returnResult.numValue = tmp1.numValue / tmp2.numValue ;
        } 
      } 
    } 
  } 
  
  void MOD_Compute( RESULT &returnResult, RESULT tmp1, RESULT tmp2 ) {
    int num1, num2 = 0 ;
    if ( tmp1.valueType == INT && tmp2.valueType == INT ) {
      num1 = tmp1.numValue ;
      num2 = tmp2.numValue ;
      
      returnResult.valueType = INT ;
      returnResult.numValue = num1 % num2 ;
    } 
  } 
  
  
  bool Rest_of_maybe_mult_exp( int &nextTokenIndex, RESULT &returnResult, 
                               RESULT preValue, RETURNID returnID ) {
    TOKENTYPE tokentype ;
    TOKENTYPE operationValue ;
    RESULT unaryExpValue ;
    RESULT r ;
    RESULT r2 ;
    RETURNID preReturnID ;
    bool isFirst = true ;
    
    
    
    if ( mIsCompute ) {
      preReturnID = returnID ;
      r = preValue ; 
    } 
    
        
    
    if ( !mIsCompute ) {
      
      PeekToken( tokentype ) ;
    } 
    
    
    else { 
      
      PassTwoPeekToken( nextTokenIndex, tokentype ) ;
    } 
    
    while ( tokentype == MUL || tokentype == DIV || tokentype == MOD ) {

      
      operationValue = tokentype ;
       
      
      if ( !mIsCompute ) {
        
        
        GetToken( tokentype ) ;
        gAllTokenStringList.push_back( gToken ) ;
      } 
      
      
      else {
        
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex = nextTokenIndex + 1 ;
      } 
      
      
      if ( Unary_exp( nextTokenIndex, unaryExpValue, returnID ) ) {    
        
        
        if ( !mIsCompute ) {
          
          PeekToken( tokentype ) ;
        } 
        
        
        else {
          
          
          if ( operationValue == MUL )
            MUL_Compute( r, r, unaryExpValue ) ;
          else if ( operationValue == DIV )
            DIV_Compute( r, r, unaryExpValue ) ;
          else
            MOD_Compute( r, r, unaryExpValue ) ;
          
          
          
          
          
          if ( isFirst ) {           
            
            if ( preReturnID.changeValue ) {
              
              
              if ( mDoExp ) {
                
                if ( preReturnID.isArray ) {
                  PPMM_Array_Compute( r2, preReturnID.returnOperationValue, 
                                      preReturnID.isIdGlobal, preReturnID.idIndex, preReturnID.arrayIndex ) ;
                } 
                
                
                else {
                  PPMM_Compute( r2, preReturnID.returnOperationValue, 
                                preReturnID.isIdGlobal, preReturnID.idIndex ) ;
                } 
              } 
              
            } 
            
            
            if ( returnID.changeValue ) {
              
              
              if ( mDoExp ) {
                
                if ( returnID.isArray ) {
                  PPMM_Array_Compute( r2, returnID.returnOperationValue, 
                                      returnID.isIdGlobal, returnID.idIndex, returnID.arrayIndex ) ;
                } 
                
                
                else {
                  PPMM_Compute( r2, returnID.returnOperationValue, returnID.isIdGlobal, returnID.idIndex ) ;
                } 
              } 
            } 
            
            
            isFirst = false ;
          } 
          
          
          else {
            
            if ( returnID.changeValue ) {
              
              
              if ( mDoExp ) { 
                
                if ( returnID.isArray ) {
                  PPMM_Array_Compute( r2, returnID.returnOperationValue, 
                                      returnID.isIdGlobal, returnID.idIndex, returnID.arrayIndex ) ;
                } 
                
                
                else {
                  PPMM_Compute( r2, returnID.returnOperationValue, returnID.isIdGlobal, returnID.idIndex ) ;
                } 
              } 
              
            } 
          } 
          
          
          PassTwoPeekToken( nextTokenIndex, tokentype ) ;
        } 
        
      } 
      
      else return false ; 
    } 
    
    
    if ( mIsCompute ) {
      
      
      if ( isFirst ) {           
        
        if ( preReturnID.changeValue ) {
          
          
          if ( mDoExp ) {
            
            if ( preReturnID.isArray ) {
              PPMM_Array_Compute( r2, preReturnID.returnOperationValue, 
                                  preReturnID.isIdGlobal, preReturnID.idIndex, preReturnID.arrayIndex ) ;
            } 
            
            
            else {
              PPMM_Compute( r2, preReturnID.returnOperationValue, 
                            preReturnID.isIdGlobal, preReturnID.idIndex ) ;
            } 
          } 
          
        } 
        
        
        isFirst = false ;
      } 
    } 
    
    
    
    if ( mIsCompute ) {
      returnResult = r ;
    } 
    
    
    return true ;
  } 
  
  bool Unary_exp( int &nextTokenIndex, RESULT &returnResult, RETURNID &returnID ) {
    TOKENTYPE tokentype ;
    TOKENTYPE signValue ;
    TOKENTYPE operationValue ;
    RESULT signedExpValue ;
    RESULT unsignedExpValue ;
    RESULT expValue ;
    RESULT r ;
    int index = 0 ;
    
    bool isIdGlobal = false ;
    int idIndex = -1 ;
    int arrayIndex = -1 ;
    
    
    returnID.changeValue = false ;
    returnID.isArray = false ;
    
    
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
        
        
        if ( Signed_unary_exp( nextTokenIndex, signedExpValue ) ) return true ;
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
              
              index = FindTokenInSymbolList( gToken.tokenID, mNewDefineSymbolList ) ;
              
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
          } 
          
          
          
          PeekToken( tokentype ) ;
          
          
          if ( tokentype == LBK ) {
            
            
            GetToken( tokentype ) ;
            gAllTokenStringList.push_back( gToken ) ;
            
            
            if ( Expression( nextTokenIndex, expValue ) ) {
              
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
                  
        
        if ( Unsigned_unary_exp( nextTokenIndex, unsignedExpValue, returnID ) ) return true ;
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
      
      PassTwoPeekToken( nextTokenIndex, tokentype ) ;
      
      if ( Sign( tokentype ) ) {
        
        
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++ ;
        
        
        signValue = tokentype ;

        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
        
        while ( Sign( tokentype ) ) {
          
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex++ ;
          
          
          if ( signValue == PLUS && tokentype == MINUS ) signValue = MINUS ;
          else if ( signValue == MINUS && tokentype == MINUS ) signValue = PLUS ;
          else if ( signValue == TRUE && tokentype == NOT ) signValue = NOT ;
          else if ( signValue == NOT && tokentype == NOT ) signValue = TRUE ;
          
          
          PassTwoPeekToken( nextTokenIndex, tokentype ) ;
        } 
        
        
        
        Signed_unary_exp( nextTokenIndex, signedExpValue ) ;
        
        
        if ( signedExpValue.valueType == INT || signedExpValue.valueType == FLOAT ) {
          if ( signValue == MINUS ) {
            signedExpValue.numValue = signedExpValue.numValue * ( -1 ) ;
          } 
        } 
        
        else if ( signedExpValue.valueType == BOOL ) {
          if ( signValue == NOT ) {
            if ( signedExpValue.boolValue ) signedExpValue.boolValue = false ;
            else signedExpValue.boolValue = true ;
          } 
        } 
        
        
        returnResult = signedExpValue ;
               
        return true ; 
      } 
      
      else if ( tokentype == PP || tokentype == MM ) {
        
        
        operationValue = tokentype ;
         
        
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++ ;
        
        
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++ ;
          
        
        ReturnNonFunctionIdLocation( isIdGlobal, idIndex, gToken.tokenID ) ;
        
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
        
        
        
        if ( tokentype == LBK ) {
          
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex++ ;
          
          
          
          Expression( nextTokenIndex, expValue ) ;
          
          
          
          GetToken( tokentype ) ;
          gAllTokenStringList.push_back( gToken ) ;
          
          
          arrayIndex = expValue.numValue ;
          
          
          if ( mDoExp ) {
            
            
            
            PPMM_Array_Compute( r, operationValue, isIdGlobal, idIndex, arrayIndex ) ;
          } 
           
        } 
        
        
        else {
          
          if ( mDoExp ) {
            
            
            
            PPMM_Compute( r, operationValue, isIdGlobal, idIndex ) ;
          } 
        } 
        
        
        returnResult = r ;
         
        return true ;
      } 
      
      
      else if ( tokentype == IDENTIFIER || tokentype == CONSTANT_INT || tokentype == CONSTANT_FLOAT 
                || tokentype == CONSTANT_CHAR
                || tokentype == CONSTANT_STRING || tokentype == CONSTANT_BOOL || tokentype == LP ) {
        
        
        
        Unsigned_unary_exp( nextTokenIndex, unsignedExpValue, returnID ) ;
        
        
        returnResult = unsignedExpValue ;
        
        return true ;
      } 
    } 
    
    return false ;
  } 
  
  bool Signed_unary_exp( int &nextTokenIndex, RESULT &returnResult ) {
    TOKENTYPE tokentype ;
    RESULT expValue ;
    vector<RESULT> actualParListValue ; 
    int index = 0 ;
    
    bool isIdGlobal = false ;
    int idIndex = -1 ;
    int arrayIndex = -1 ;
    RESULT r ;
    
    
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
            
            index = FindTokenInSymbolList( gToken.tokenID, mNewDefineSymbolList ) ;
            
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
              
              
              if ( !Actual_parameter_list( nextTokenIndex, actualParListValue ) ) {
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
            
            if ( Expression( nextTokenIndex, expValue ) ) {
              
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
        
        if ( Expression( nextTokenIndex, expValue ) ) {
          
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
      
      PassTwoGetToken( nextTokenIndex, tokentype ) ;
      nextTokenIndex++ ;
      
      
      if ( tokentype == IDENTIFIER ) {
        
        
        ReturnNonFunctionIdLocation( isIdGlobal, idIndex, gToken.tokenID ) ;
         
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
        
        
        if ( tokentype == LP || tokentype == LBK ) {
          
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex++ ;
          
          
          
          if ( tokentype == LP ) {
          /*  
            
            PeekToken( tokentype ) ;
            
            
            if ( tokentype == IDENTIFIER || tokentype == PP || tokentype == MM 
                 || Sign( tokentype ) || tokentype == CONSTANT_INT 
                 || tokentype == CONSTANT_FLOAT || tokentype == CONSTANT_CHAR 
                 || tokentype == CONSTANT_STRING || tokentype == CONSTANT_BOOL
                 || tokentype == LP ) {
              
              
              if ( !Actual_parameter_list( nextTokenIndex, actualParListValue ) ) {
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
          */
            
            return true ;
          } 
          
          
          else {
            
            
            Expression( nextTokenIndex, expValue ) ;
            
            
            
            PassTwoGetToken( nextTokenIndex, tokentype ) ;
            nextTokenIndex++ ;
            
            
            arrayIndex = expValue.numValue ;
            
            
            ReturnIdArrayValue( r, isIdGlobal, idIndex, arrayIndex ) ;
          } 
        } 
        
        
        else {
          
          ReturnIdValue( r, isIdGlobal, idIndex ) ;
        } 
        
        
        returnResult = r ;
  
        return true ;
      } 
      
      else if ( tokentype == CONSTANT_INT || tokentype == CONSTANT_FLOAT || 
                tokentype == CONSTANT_CHAR || tokentype == CONSTANT_STRING || tokentype == CONSTANT_BOOL ) {
        
        
        ReturnConstValue( r, gToken ) ;
        
        
        returnResult = r ;
         
        return true ;
      } 
      
      else if ( tokentype == LP ) {
        
        
        Expression( nextTokenIndex, expValue ) ;
        
        
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++ ;
               
        
        returnResult = expValue ;
         
        return true ; 
      } 
    } 
    
    return false ;
  } 
  
  bool Unsigned_unary_exp( int &nextTokenIndex, RESULT &returnResult, RETURNID &returnID ) {
    TOKENTYPE tokentype ;
    TOKENTYPE operationValue ;
    RESULT expValue ;
    vector<RESULT> actualParListValue ; 
    
    int index = 0 ;
    
    bool isIdGlobal = false ;
    int idIndex = -1 ;
    int arrayIndex = -1 ;
    RESULT r ;
    
    
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
            
            index = FindTokenInSymbolList( gToken.tokenID, mNewDefineSymbolList ) ; 
            
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
              
              
              if ( !Actual_parameter_list( nextTokenIndex, actualParListValue ) ) {
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
            
            
            if ( Expression( nextTokenIndex, expValue ) ) {
              
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
        
        if ( Expression( nextTokenIndex, expValue ) ) {
          
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
      
      PassTwoGetToken( nextTokenIndex, tokentype ) ;
      nextTokenIndex++ ;
        
      
      if ( tokentype == IDENTIFIER ) {

        
        ReturnNonFunctionIdLocation( isIdGlobal, idIndex, gToken.tokenID ) ;
     
        
        PassTwoPeekToken( nextTokenIndex, tokentype ) ;
        
        
        if ( tokentype == LP || tokentype == LBK || tokentype == PP || tokentype == MM ) {
          
          
          
          PassTwoGetToken( nextTokenIndex, tokentype ) ;
          nextTokenIndex++ ;
          
          
          if ( tokentype == LP ) {
          /*
            
            PeekToken( tokentype ) ;
            
            
            if ( tokentype == IDENTIFIER || tokentype == PP || tokentype == MM 
                 || Sign( tokentype ) || tokentype == CONSTANT_INT 
                 || tokentype == CONSTANT_FLOAT || tokentype == CONSTANT_CHAR 
                 || tokentype == CONSTANT_STRING || tokentype == CONSTANT_BOOL
                 || tokentype == LP ) {
              
              
              if ( !Actual_parameter_list( nextTokenIndex, actualParListValue ) ) {
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
          */
            
            return true ;
          } 
          
          else if ( tokentype == LBK ) {
            
            
            
            Expression( nextTokenIndex, expValue ) ;
            
            
            
            PassTwoGetToken( nextTokenIndex, tokentype ) ;
            nextTokenIndex++ ;
            
            
            arrayIndex = expValue.numValue ;
            
            
            PassTwoPeekToken( nextTokenIndex, tokentype ) ;
            
            
            
            if ( tokentype == PP || tokentype == MM ) {
              
              operationValue = tokentype ;
               
              
              
              PassTwoGetToken( nextTokenIndex, tokentype ) ;
              nextTokenIndex++ ;
              
              
              returnID.changeValue = true ;
              returnID.isArray = true ;
              returnID.isIdGlobal = isIdGlobal ;
              returnID.idIndex = idIndex ;
              returnID.arrayIndex = arrayIndex ;
              returnID.returnOperationValue = operationValue ;
            } 
            
            
            ReturnIdArrayValue( r, isIdGlobal, idIndex, arrayIndex ) ;
            
            
            returnResult = r ;
            
            return true ; 
          } 
          
          
          
          else {
            
            operationValue = tokentype ;
               
            
            returnID.changeValue = true ;
            returnID.isArray = false ;
            returnID.isIdGlobal = isIdGlobal ;
            returnID.idIndex = idIndex ;
            returnID.returnOperationValue = operationValue ;
            
            
            ReturnIdValue( r, isIdGlobal, idIndex ) ;
            
            
            returnResult = r ;
            
            return true ;
          } 
        } 
        
        
        
        else {
          
          ReturnIdValue( r, isIdGlobal, idIndex ) ;
          
          
          returnResult = r ;
          
          return true ;
        } 
        
        return true;
      } 
      
      else if ( tokentype == CONSTANT_INT || tokentype == CONSTANT_FLOAT || 
                tokentype == CONSTANT_CHAR || tokentype == CONSTANT_STRING || tokentype == CONSTANT_BOOL ) {
        
        
        ReturnConstValue( r, gToken ) ;
        
        
        returnResult = r ;

        return true ;
      } 
      
      else if ( tokentype == LP ) {
        
        
        Expression( nextTokenIndex, expValue ) ;
        
        
        
        PassTwoGetToken( nextTokenIndex, tokentype ) ;
        nextTokenIndex++ ;
        
        
        returnResult = expValue ;

        return true ;
      } 
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
  LOCAL firstStack ;
  
  s.TokenInit( gToken ) ;
  s.TokenInit( gPeekToken ) ;
  
  cin >> uTestNum ;
  cout << "Our-C running ..." << endl ;
  
  
  gCallStack.push_back( firstStack ) ;
  
  while ( !isQuit ) {
    try {
      
      cout << "> " ;
      
      
      if ( s.A_user_input( userInputFirstTokenLine, userIntpuFirstTOkenColumn, isQuit ) ) {
        
        if ( isQuit ) {
          cout << "Our-C exited ..." ;
          return 0 ;
        } 
        
        
        s.mIsCompute = true ;       
        s.A_user_input( userInputFirstTokenLine, userIntpuFirstTOkenColumn, isQuit ) ;
        
        
        s.mIsCompute = false ; 
        s.TokenInit( gToken ) ;
        s.TokenInit( gPeekToken ) ;
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
      cout << "Line " << line << " : " << "unexpected token '" << exp2->message << "'" << endl;
      s.SkipErrorLine() ;
    } 
    catch ( Exception3 * exp3 ) {
      
      
      line = gAllTokenStringList[gAllTokenStringList.size()-1].tokenLine  - pointToTheLastTokenLine ;
      if ( line == 0  ) line = 1 ;
      cout << "Line " << line << " : "  << "undefined identifier '" << exp3->message << "'" << endl;
      s.SkipErrorLine() ;
    } 
    catch ( Exception4 * exp4 ) {
      cout << exp4->message << endl;
      s.SkipErrorLine() ;
    } 
    
    
    pointToTheLastTokenLine = gAllTokenStringList[gAllTokenStringList.size()-1].tokenLine ; 
    s.FunctionInit( s.mNewDefineFunc ) ; 
    s.mNewDefineSymbolList.clear() ; 
    if ( gPeekToken.tokenID.empty() && gisEOF ) isQuit = true ;
  } 
  
  
  return 0;
} 
