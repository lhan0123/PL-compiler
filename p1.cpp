# include <iostream>
# include <string>
# include <string.h>
# include <vector>
# include <cstdlib>
# include <stdio.h>
# include <iomanip>

using namespace std;

int gLine = 1; 
int gColumn = 1 ; 
int gcurLine = 1 ; 
int gcurColumn = 1 ; 
int gtokenLine = 1 ; 
int gtokenColumn = 1 ; 

bool gisEOF = false ;  

char gcurchar = ' ' ;   
char gnextchar = '\0' ;  

string gDefinetionStatement = "" ;

enum TOKENTYPE
{
  IDENT,  
  INTEGER, 
  FLOAT,   
  ADD,     
  MINUS,   
  MUL,     
  DIV,     
  LP,       
  RP,      
  EQ,     
  GT,     
  LT,     
  GE,   
  LE,   
  NEQ,  
  STATE_END, 
  DEFINE, 
  ISEOF, 
  NONE  
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
  vector<TOKEN_CHAR> token_char_string ;
};

TOKEN gToken ;
TOKEN gPeekToken ;

struct SYMBOL{
  string symbolName ;
  int symbolType ;
  double symbolValue ;
};

vector<SYMBOL> gSymbolList ; 

struct ERRORTYPE{
  bool isError1 ;  
  bool isError2  ;  
  bool isError3 ;  
  bool isError4  ;  
  bool isError5 ;  
  string errorToken  ; 
  int errorline ;      
};

# define value_int 9990333 
# define value_float  3303999
# define value_bool 2022999
# define value_not_define 9992022

struct VALUERESULT{
  int valueType ;     
  double value  ; 
  bool value_bool_ans ; 
};


void Global_Token_init() {
  gToken.tokenID = "" ;
  gToken.tokenLine = 0 ;
  gToken.tokenColumn = 0 ;
  gToken.tokentype = NONE ;
  gToken.token_char_string.clear() ;
} 

void Global_Peek_Token_init() {
  gPeekToken.tokenID = "" ;
  gPeekToken.tokenLine = 0 ;
  gPeekToken.tokenColumn = 0 ;
  gPeekToken.tokentype = NONE ;
  gPeekToken.token_char_string.clear() ;
} 

int StringToInt( string str ) {
  return atoi( str.c_str() ) ;
} 

double StringToFloat( string str ) {
  int i = 0 ;
  string num = "" ;
  string tmp = "" ;
  string floatNum = "" ;
  double floatValue = 0.000 ;
  double numTen = 0.1 ;
  bool isDone = false ;
  bool hasDot = false ;
  
  for ( int i = 0 ; i < str.size() ; i++ ) {
    if ( str.at( i ) == '.' ) {
      hasDot = true ;
    } 
  } 
  
  if ( hasDot ) {
    while ( str.at( i ) != '.' && ! isDone ) {
      num = num + str.at( i ) ;
      i ++ ;
      if ( i >= str.size() ) {
        isDone = true ;
      } 
    } 
    
    numTen = 1 ;
    for ( int j = num.size()-1 ; j >= 0  ; j-- ) {
      tmp = tmp + num.at( j ) ;
      floatValue = floatValue + ( StringToInt( tmp ) * numTen ) ;
      numTen = numTen * 10 ;
      tmp.clear() ;
    } 
    
    
    num.clear() ;
    
    numTen = 0.1 ;
    for ( int j = i + 1 ; j < str.size() ; j++ ) {
      num = num + str.at( j ) ;
      floatValue = floatValue + ( StringToInt( num ) * numTen ) ;
      num.clear() ;
      numTen = numTen * 0.1 ;  
    } 
  } 
  
  else {
    numTen = 1 ;
    for ( int j = str.size()-1 ; j >= 0  ; j-- ) {
      tmp = tmp + str.at( j ) ;
      floatValue = floatValue + ( StringToInt( tmp ) * numTen ) ;
      numTen = numTen * 10 ;
      tmp.clear() ;
    } 
  } 
  

  return floatValue ;
} 


string CopyString( string token ) {
  string str = "" ;
  for ( int i = 0 ; i < token.size() ; i++ ) {
    str = str + token[i] ;
  } 

  return str ;
} 


bool Get_next_ch( char &ch ) {
  
  if ( scanf( "%c", &ch) == EOF ) {
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
  TOKEN_CHAR tc ;
  string token = "" ;
  bool isBreak = false ;

  tc.tokenCHAR = gcurchar ;
  tc.line = gtokenLine ;
  tc.column = gtokenColumn ;
  t.token_char_string.push_back( tc ) ;

  token = token + gcurchar ; 		
  if ( Get_next_ch( gnextchar ) ) { 

    while ( ( ( gnextchar >= 'a' && gnextchar <= 'z' ) || ( gnextchar >= 'A' && gnextchar <= 'Z' ) 
               || ( gnextchar >= '0' && gnextchar <= '9' ) || gnextchar == '_' ) && !isBreak ) {
      gcurchar = gnextchar ; 

      tc.tokenCHAR = gcurchar ;
      tc.line = gcurLine ;
      tc.column = gcurColumn ;
      t.token_char_string.push_back( tc ) ;

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
  t.tokenLine = t.token_char_string[0].line ;
  t.tokenColumn = t.token_char_string[0].column ;	
} 


void Get_NUM_token( TOKEN &t ) {
  TOKEN_CHAR tc ;
  string token = "" ;
  bool isBreak = false ;

  tc.tokenCHAR = gcurchar ;
  tc.line = gtokenLine ;
  tc.column = gtokenColumn ;
  t.token_char_string.push_back( tc ) ;

  token = token + gcurchar ; 
  if ( Get_next_ch( gnextchar ) ) { 

    while ( ( gnextchar >= '0' && gnextchar <= '9' ) && !isBreak ) {
      gcurchar = gnextchar ; 

      tc.tokenCHAR = gcurchar ;
      tc.line = gcurLine ;
      tc.column = gcurColumn ;
      t.token_char_string.push_back( tc ) ;

      token = token + gcurchar ; 
      if ( !Get_next_ch( gnextchar ) ) { 
        gisEOF = true ; 
        isBreak = true ; 
      } 
    } 

    isBreak = false ;

    if ( !gisEOF && gnextchar == '.' ) {
      gcurchar = gnextchar ;

      tc.tokenCHAR = gcurchar ;
      tc.line = gcurLine ;
      tc.column = gcurColumn ;
      t.token_char_string.push_back( tc ) ;

      token = token + gcurchar ;
      if ( Get_next_ch( gnextchar ) ) { 

        while ( ( gnextchar >= '0' && gnextchar <= '9' ) && !isBreak ) {
          gcurchar = gnextchar ; 

          tc.tokenCHAR = gcurchar ;
          tc.line = gcurLine ;
          tc.column = gcurColumn ;
          t.token_char_string.push_back( tc ) ;

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
  t.tokenLine = t.token_char_string[0].line ;
  t.tokenColumn = t.token_char_string[0].column ;

} 


void Get_Special_ch_token( TOKEN &t ) {
  TOKEN_CHAR tc ;
  string token = "" ;
  bool isBreak = false ;

  
  if ( gcurchar == ':' ) {
    tc.tokenCHAR = gcurchar ;
    tc.line = gtokenLine ;
    tc.column = gtokenColumn ;
    t.token_char_string.push_back( tc ) ;

    token = token + gcurchar ; 
    if ( Get_next_ch( gnextchar ) ) { 
      if ( gnextchar == '=' ) { 
        gcurchar = gnextchar ; 

        tc.tokenCHAR = gcurchar ;
        tc.line = gcurLine ;
        tc.column = gcurColumn ;
        t.token_char_string.push_back( tc ) ;

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

  
  else if ( gcurchar == '+' || gcurchar == '-' || gcurchar == '*' 
            || gcurchar == '=' || gcurchar == '(' || gcurchar == ')' || gcurchar == ';' ) {

    tc.tokenCHAR = gcurchar ;
    tc.line = gtokenLine ;
    tc.column = gtokenColumn ;
    t.token_char_string.push_back( tc ) ;

    token = token + gcurchar ; 
    if ( !Get_next_ch( gnextchar ) ) { 
      gisEOF = true ; 
    } 
  } 

  
  else if ( gcurchar == '<' ) {  
    tc.tokenCHAR = gcurchar ;
    tc.line = gtokenLine ;
    tc.column = gtokenColumn ;
    t.token_char_string.push_back( tc ) ;

    token = token + gcurchar ; 
    if ( Get_next_ch( gnextchar ) ) {  
      if ( gnextchar == '>' || gnextchar == '=' ) {         
        gcurchar = gnextchar ; 

        tc.tokenCHAR = gcurchar ;
        tc.line = gcurLine ;
        tc.column = gcurColumn ;
        t.token_char_string.push_back( tc ) ;

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
    tc.tokenCHAR = gcurchar ;
    tc.line = gtokenLine ;
    tc.column = gtokenColumn ;
    t.token_char_string.push_back( tc ) ;

    token = token + gcurchar ; 
    if ( Get_next_ch( gnextchar ) ) { 
      if ( gnextchar == '=' ) { 
        gcurchar = gnextchar ; 

        tc.tokenCHAR = gcurchar ;
        tc.line = gcurLine ;
        tc.column = gcurColumn ;
        t.token_char_string.push_back( tc ) ;

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

  
  else if ( gcurchar == '.' ) {
    tc.tokenCHAR = gcurchar ;
    tc.line = gtokenLine ;
    tc.column = gtokenColumn ;
    t.token_char_string.push_back( tc ) ;

    token = token + gcurchar ; 
    if ( Get_next_ch( gnextchar ) ) { 
      
      while ( ( gnextchar >= '0' && gnextchar <= '9' ) && !isBreak ) {
        gcurchar = gnextchar ; 

        tc.tokenCHAR = gcurchar ;
        tc.line = gcurLine ;
        tc.column = gcurColumn ;
        t.token_char_string.push_back( tc ) ;

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

  
  
  else if ( gcurchar == '/' ) { 
    tc.tokenCHAR = gcurchar ;
    tc.line = gtokenLine ;
    tc.column = gtokenColumn ;
    t.token_char_string.push_back( tc ) ;

    token = token + gcurchar ; 
  } 

	
	else {
    tc.tokenCHAR = gcurchar ;
    tc.line = gtokenLine ;
    tc.column = gtokenColumn ;
    t.token_char_string.push_back( tc ) ;

    token = token + gcurchar ; 
    if ( !Get_next_ch( gnextchar ) )  
      gisEOF = true ;	
  } 

  t.tokenID = token ;
  t.tokenLine = t.token_char_string[0].line ;
  t.tokenColumn = t.token_char_string[0].column ;

} 





bool Gettoken( TOKEN &t ) {
  string token = "" ;  

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
    
    gcurchar = gnextchar ; 
    return true ; 
  } 

  return false ;	
} 


int IsSymbolExist( string tokenID ) {
  for ( int i = 0 ; i < gSymbolList.size() ; i++ ) {
    if ( tokenID.compare( gSymbolList[i].symbolName ) == 0 ) {
      return i ;
    } 
  } 

  return -1 ;
} 


void PrintSymbolList() {
  cout << "Symbol List:" << endl ; 
  for ( int i = 0 ; i < gSymbolList.size() ; i++ ) {
    cout << gSymbolList[i].symbolName << "\t" << gSymbolList[i].symbolValue << endl ;
  } 
} 


class SYNTAX{
public:

  ERRORTYPE mError ; 
  bool mIsCompute  ; 

  void Error_init() {
    mError.isError1 = false ;
    mError.isError2 = false ;
    mError.isError3 = false ;
    mError.isError4 = false ;
    mError.isError5 = false ;
    mError.errorToken = "" ;
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

  
  bool IsInteger( string tokenID ) {
    for ( int i = 0 ; i < tokenID.size() ; i++ ) {
      if ( tokenID.at( i ) < '0' || tokenID.at( i ) > '9' ) {
        return false ;
      } 
    } 

    return true ;
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
    if ( tokenID.compare( ":=" ) == 0 ) tokentype = DEFINE ;
    else if ( tokenID.compare( ";" ) == 0 ) tokentype = STATE_END ;
    else if ( tokenID.compare( "<>" ) == 0 ) tokentype = NEQ ;
    else if ( tokenID.compare( "<=" ) == 0 ) tokentype = LE ;
    else if ( tokenID.compare( ">=" ) == 0 ) tokentype = GE ;
    else if ( tokenID.compare( "<" ) == 0 ) tokentype = LT ;
    else if ( tokenID.compare( ">" ) == 0 ) tokentype = GT ;
    else if ( tokenID.compare( "=" ) == 0 ) tokentype = EQ ;
    else if ( tokenID.compare( "(" ) == 0 ) tokentype = LP ;
    else if ( tokenID.compare( ")" ) == 0 ) tokentype = RP ;
    else if ( tokenID.compare( "+" ) == 0 ) tokentype = ADD ;
    else if ( tokenID.compare( "-" ) == 0 ) tokentype = MINUS ;
    else if ( tokenID.compare( "*" ) == 0 ) tokentype = MUL ;
    else if ( tokenID.compare( "/" ) == 0 ) tokentype = DIV ;
    else if ( IsFloat( tokenID ) ) tokentype = FLOAT ;
    else if ( IsInteger( tokenID ) ) tokentype = INTEGER ;
    else if ( IsIdentifier( tokenID ) ) tokentype = IDENT ;
    else tokentype = NONE ;

  } 
  
  
  void GetTokenAndType( TOKEN &t ) {
    
    if ( Gettoken( t ) ) {
       GetTokenType( t.tokenID, t.tokentype )  ; 
    } 
    
    else {
      t.tokentype = ISEOF ; 
    } 
  } 
  

  
  
  
  void PeekToken( TOKEN &t ) {	
    
    if ( Gettoken( t ) ) { 
       GetTokenType( t.tokenID, t.tokentype )  ; 
    } 
    
    else {
      t.tokentype = ISEOF ; 
    } 
  } 

  
  bool IsEnd( string firstToken ) {

    if ( firstToken.compare( "quit" ) == 0 )
      return true ;

    return false ;
  } 
  
  
  bool Factor( vector<TOKEN> &inputString, VALUERESULT &v ) {
    TOKENTYPE tokentype ; 
    TOKENTYPE add_or_minus ; 

    int symbolIndex = -1 ;
    VALUERESULT expValue ;
    
    
    if ( gPeekToken.tokenID.empty() ) {
      
      Global_Token_init() ;
      
      
      GetTokenAndType( gToken ) ; 
      
      
      tokentype = gToken.tokentype ;
    } 
    
    
    else {
      Global_Token_init() ; 
      
      
      gToken.tokenID = CopyString( gPeekToken.tokenID ) ;
      gToken.tokenLine = gPeekToken.tokenLine ;
      gToken.tokenColumn = gPeekToken.tokenColumn ;
      gToken.tokentype = gPeekToken.tokentype ;
      for ( int i = 0 ; i < gPeekToken.token_char_string.size() ; i++ ) {
        gToken.token_char_string.push_back( gPeekToken.token_char_string[i] ) ;
      } 

      
      Global_Peek_Token_init() ;
      
      
      tokentype = gToken.tokentype ;
    } 
    

    
    if ( tokentype == IDENT ) {
      
      inputString.push_back( gToken ) ; 
      
      
      
      
      symbolIndex = IsSymbolExist( gToken.tokenID ) ;  

      
      if ( symbolIndex == -1 ) {
        
        
        mError.isError3 = true ;
        mError.errorToken = gToken.tokenID ;
        mError.errorline = gToken.tokenLine ;
        return false ;
        
      } 

      
      else {

        
        v.valueType = gSymbolList[symbolIndex].symbolType ; 
        v.value = gSymbolList[symbolIndex].symbolValue ; 
      } 
      
      
      
      return true ;  
    } 

    
    else if ( tokentype == ADD || tokentype == MINUS ) {
      
      inputString.push_back( gToken ) ; 
      add_or_minus = tokentype ; 
      
      
      
      
      if ( gPeekToken.tokenID.empty() ) {
        Global_Token_init() ; 
        GetTokenAndType( gToken ) ; 
        tokentype = gToken.tokentype ;  
      } 
      
      
      else {
        Global_Token_init() ; 
        
        
        gToken.tokenID = CopyString( gPeekToken.tokenID ) ;
        gToken.tokenLine = gPeekToken.tokenLine ;
        gToken.tokenColumn = gPeekToken.tokenColumn ;
        gToken.tokentype = gPeekToken.tokentype ;
        for ( int i = 0 ; i < gPeekToken.token_char_string.size() ; i++ ) {
          gToken.token_char_string.push_back( gPeekToken.token_char_string[i] ) ;
        } 
        
        
        Global_Peek_Token_init() ;
        
        
        tokentype = gToken.tokentype ;
      } 
             
      
      if ( tokentype == INTEGER || tokentype == FLOAT ) {
           
        inputString.push_back( gToken ) ; 
        
        
        
        if ( tokentype == INTEGER ) {
          
          
          v.valueType = value_int ;

          
          if  ( add_or_minus == MINUS ) 
            v.value = StringToFloat( gToken.tokenID ) * ( -1 ) ;
            
          else 
            v.value = StringToFloat( gToken.tokenID ) ;                   
        } 

        
        else {
          
          v.valueType = value_float ;

          
          if ( add_or_minus == MINUS ) 
            v.value = StringToFloat( gToken.tokenID ) * ( -1 ) ;
            
          else 
            v.value = StringToFloat( gToken.tokenID ) ;
        } 
        
        
        
        return true ;   
      } 
      
      
      else {
        
        
        if ( tokentype == NONE ) {
          mError.isError1 = true ;
          mError.errorToken = gToken.tokenID ;
          mError.errorline = gToken.tokenLine ;
          return false ;
        } 

        
        else {
          mError.isError2 = true ;
          mError.errorToken = gToken.tokenID ;
          mError.errorline = gToken.tokenLine ;
          return false ;
        } 
      } 
    } 

    
    else if ( tokentype == INTEGER || tokentype == FLOAT ) {
      
      inputString.push_back( gToken ) ; 
      
      
      
      if ( tokentype == INTEGER ) {
        
        v.valueType = value_int ;

        
        v.value = StringToFloat( gToken.tokenID ) ;
        
      } 

      
      else {
        
        v.valueType = value_float ;

        
        v.value = StringToFloat( gToken.tokenID ) ;
      } 
      
      
      
      return true ;
    } 

    
    else if( tokentype == LP ) {
      
      inputString.push_back( gToken ) ; 
      
      if ( ArithExp( inputString, expValue ) ) { 
        
        if ( gPeekToken.tokenID.empty() ) {
          Global_Token_init() ; 
          GetTokenAndType( gToken ) ; 
          tokentype = gToken.tokentype ;  
        } 
        
        
        else {
          Global_Token_init() ; 
          
          
          gToken.tokenID = CopyString( gPeekToken.tokenID ) ;
          gToken.tokenLine = gPeekToken.tokenLine ;
          gToken.tokenColumn = gPeekToken.tokenColumn ;
          gToken.tokentype = gPeekToken.tokentype ;
          for ( int i = 0 ; i < gPeekToken.token_char_string.size() ; i++ ) {
            gToken.token_char_string.push_back( gPeekToken.token_char_string[i] ) ;
          } 
          
          Global_Peek_Token_init() ;
               
          tokentype = gToken.tokentype ;
        } 
        
        
        if ( tokentype == RP ) {
          
          inputString.push_back( gToken ) ; 
          
          v.valueType = expValue.valueType ; 
          v.value = expValue.value ; 

          return true ;
        } 
        
        
        else {
          
          if ( tokentype == NONE ) {
            mError.isError1 = true ;
            mError.errorToken = gToken.tokenID ;
            mError.errorline = gToken.tokenLine ;
            return false ;
          } 
          
          
          else {
            mError.isError2 = true ;
            mError.errorToken = gToken.tokenID ;
            mError.errorline = gToken.tokenLine ;
            return false ;
          } 
        } 
          
        return false ;
      } 

      else 
        return false ;
    } 
     
    
    
    else if ( tokentype == NONE ) {
      mError.isError1 = true ;
      mError.errorToken = gToken.tokenID ;
      mError.errorline = gToken.tokenLine ;
      return false ;
    } 

    
    else {
      mError.isError2 = true ;
      mError.errorToken = gToken.tokenID ;
      mError.errorline = gToken.tokenLine ;
      return false ;
    } 
  } 
  
  
  bool Term( vector<TOKEN> &inputString, VALUERESULT &v ) {
    bool isCorrect = false ;
    TOKENTYPE tokentype ; 
    VALUERESULT factor1Value ;
    VALUERESULT factor2Value ;
    bool isFloat = false ;
    int temp1 = 0 ;
    int temp2 = 0 ;
    int symbolIndex = -1 ;

    if ( Factor( inputString, factor1Value ) ) { 

      if ( factor1Value.valueType == value_float ) isFloat = true ; 
      
      do  {
  
        if ( gPeekToken.tokenID.empty() ) {                  
          PeekToken( gPeekToken ) ;
          
          
          tokentype = gPeekToken.tokentype ;  
        } 
       
        
        else {

          tokentype = gPeekToken.tokentype ;
          
        } 
        
        
        if ( tokentype == ISEOF || tokentype == STATE_END || ( tokentype != MUL && tokentype != DIV ) ) {
          
          isCorrect = true ;

          v.value = factor1Value.value ;
                    
          if ( isFloat ) v.valueType = value_float ;

          else v.valueType = value_int ;

          return true ;
        } 

        
        if ( gPeekToken.tokenID.empty() ) {
          Global_Token_init() ; 
          GetTokenAndType( gToken ) ; 
          tokentype = gToken.tokentype ;  
        } 
        
        
        else {
          Global_Token_init() ; 
          
          
          gToken.tokenID = CopyString( gPeekToken.tokenID ) ;
          gToken.tokenLine = gPeekToken.tokenLine ;
          gToken.tokenColumn = gPeekToken.tokenColumn ;
          gToken.tokentype = gPeekToken.tokentype ;
          for ( int i = 0 ; i < gPeekToken.token_char_string.size() ; i++ ) {
            gToken.token_char_string.push_back( gPeekToken.token_char_string[i] ) ;
          } 
          
          
          Global_Peek_Token_init() ;
          
          
          tokentype = gToken.tokentype ;
        } 
        
        inputString.push_back( gToken ) ; 
             
        if ( Factor( inputString, factor2Value ) ) { 
          isCorrect = true ;
        } 

        else { 
          isCorrect = false ;
          
          
          return false ;
        } 
        
        
        
        if ( tokentype == MUL ) {
          
          if ( factor2Value.valueType == value_float ) isFloat = true ;
          
          
          factor1Value.value = factor1Value.value * factor2Value.value ;
        } 

        
        else { 

          
          if ( factor2Value.value == 0 ) {
            
            mError.isError5 = true ;
            
            
            mError.errorline = inputString[inputString.size()-1].tokenLine ;
            return false ;
          } 

          
          else {
            
            if ( factor2Value.valueType == value_float ) isFloat = true ;
            
            
            if ( !isFloat ) {
              temp1 = factor1Value.value ; 
              temp2 = factor2Value.value ; 
              
              
              
              if ( temp1 % temp2 != 0 ) {
                isFloat = true ;
              } 
            } 
             
            
            factor1Value.value = factor1Value.value / factor2Value.value ;
          } 
        } 
        
        
      } while ( isCorrect ) ;
    } 
   
    
    return false ;	
  } 


  
  bool ArithExp(  vector<TOKEN> &inputString, VALUERESULT &v ) {
    bool isCorrect = false ;
    TOKENTYPE tokentype ; 
    VALUERESULT term1Value ;
    VALUERESULT term2Value ;
    bool isFloat = false ;
    int symbolIndex = -1 ;

    if ( Term( inputString, term1Value ) ) { 

      
      
      if ( term1Value.valueType == value_float ) isFloat = true ; 
      

      do {

        
        
        
        if ( gPeekToken.tokenID.empty() ) {
          
          
          PeekToken( gPeekToken ) ;
          
          
          tokentype = gPeekToken.tokentype ;  
        } 
        
        
        else {
          
          
          tokentype = gPeekToken.tokentype ;
          
        } 
        
        
        if ( tokentype == ISEOF || tokentype == STATE_END || ( tokentype != ADD && tokentype != MINUS ) ) {
          isCorrect = true ;

          
          
          v.value = term1Value.value ;
          
          
          if ( isFloat ) v.valueType = value_float ;

          
          else v.valueType = value_int ;
          
          
          
          return true ;
        } 
        
        
        
        
        if ( gPeekToken.tokenID.empty() ) {
          Global_Token_init() ; 
          GetTokenAndType( gToken ) ; 
          tokentype = gToken.tokentype ;  
        } 
        
        
        else {
          Global_Token_init() ; 
          
          
          gToken.tokenID = CopyString( gPeekToken.tokenID ) ;
          gToken.tokenLine = gPeekToken.tokenLine ;
          gToken.tokenColumn = gPeekToken.tokenColumn ;
          gToken.tokentype = gPeekToken.tokentype ;
          for ( int i = 0 ; i < gPeekToken.token_char_string.size() ; i++ ) {
            gToken.token_char_string.push_back( gPeekToken.token_char_string[i] ) ;
          } 
          
          
          Global_Peek_Token_init() ;
          
          
          tokentype = gToken.tokentype ;
        } 
        
        inputString.push_back( gToken ) ; 
        

        if ( Term( inputString, term2Value ) ) { 
          isCorrect = true ;
        } 

        else { 
          isCorrect = false ;
          return false ; 
        } 

        
        
        if ( tokentype == ADD ) {

          
          if ( term2Value.valueType == value_float ) isFloat = true ;
          
          term1Value.value = term1Value.value + term2Value.value ;

        } 

        
        else {
          
          if ( term2Value.valueType == value_float ) isFloat = true ;
          
          term1Value.value = term1Value.value - term2Value.value ;
        } 
        

      } while ( isCorrect ) ; 
    } 

    
    return false ;

  } 
  
  
  bool Not_ID_StartFactor( vector<TOKEN> &inputString, VALUERESULT &v ) {
    TOKENTYPE tokentype ; 
    TOKENTYPE add_or_minus ; 
    VALUERESULT expValue ;
    int temp1 = 0 ;
    int temp2 = 0 ;
    int symbolIndex = -1 ;
    
    
    
    
    if ( gPeekToken.tokenID.empty() ) {
      Global_Token_init() ; 
      GetTokenAndType( gToken ) ; 
      tokentype = gToken.tokentype ;  
    } 
    
    
    else {
      Global_Token_init() ; 
      
      
      gToken.tokenID = CopyString( gPeekToken.tokenID ) ;
      gToken.tokenLine = gPeekToken.tokenLine ;
      gToken.tokenColumn = gPeekToken.tokenColumn ;
      gToken.tokentype = gPeekToken.tokentype ;
      for ( int i = 0 ; i < gPeekToken.token_char_string.size() ; i++ ) {
        gToken.token_char_string.push_back( gPeekToken.token_char_string[i] ) ;
      } 
      
      
      Global_Peek_Token_init() ;
      
      
      tokentype = gToken.tokentype ;
    } 
        
    
    if ( tokentype == ADD || tokentype == MINUS ) {
      
      inputString.push_back( gToken ) ; 
      
      add_or_minus = tokentype ; 
      
      
      
    
      
      if ( gPeekToken.tokenID.empty() ) {
        Global_Token_init() ; 
        GetTokenAndType( gToken ) ; 
        tokentype = gToken.tokentype ;  
      } 
      
      
      else {
        Global_Token_init() ; 
        
        
        gToken.tokenID = CopyString( gPeekToken.tokenID ) ;
        gToken.tokenLine = gPeekToken.tokenLine ;
        gToken.tokenColumn = gPeekToken.tokenColumn ;
        gToken.tokentype = gPeekToken.tokentype ;
        for ( int i = 0 ; i < gPeekToken.token_char_string.size() ; i++ ) {
          gToken.token_char_string.push_back( gPeekToken.token_char_string[i] ) ;
        } 
        
        
        Global_Peek_Token_init() ;
        
        
        tokentype = gToken.tokentype ;
      } 
      

      if ( tokentype == INTEGER || tokentype == FLOAT ) { 
        
        
        inputString.push_back( gToken ) ;

        
        
        if ( tokentype == INTEGER ) {
          
          v.valueType = value_int ;

          
          
          if ( add_or_minus == MINUS )            
            v.value  = StringToFloat( gToken.tokenID ) * ( -1 ) ;
            
          
          else 
            v.value  = StringToFloat( gToken.tokenID ) ;

            
        } 

        
        else {
          
          v.valueType = value_float ;

          
          if ( add_or_minus == MINUS ) 
            v.value = StringToFloat( gToken.tokenID ) * ( -1 ) ;
          else 
            v.value = StringToFloat( gToken.tokenID );
        } 
        
         
        
        return true ;
      } 
      
      
      else {

        
        if ( tokentype == NONE ) {
          mError.isError1 = true ;
          mError.errorToken = gToken.tokenID ;
          mError.errorline = gToken.tokenLine ;
          return false ;
        } 

        
        else {
          mError.isError2 = true ;
          mError.errorToken = gToken.tokenID ;
          mError.errorline = gToken.tokenLine ;
          return false ;
        } 
      } 
    } 

    
    else if ( tokentype == INTEGER || tokentype == FLOAT ) {
      
      inputString.push_back( gToken ) ;

      
      
      
      if ( tokentype == INTEGER ) {
        
        v.valueType = value_int ;

        
        v.value = StringToFloat( gToken.tokenID ) ;
      } 

      
      else {
        
        v.valueType = value_float ;

        
        v.value = StringToFloat( gToken.tokenID ) ;
      } 
      

      
      return true ;
    } 

    
    else if( tokentype == LP ) {

      
      inputString.push_back( gToken ) ;

      if ( ArithExp( inputString, expValue ) ) { 

        if ( gPeekToken.tokenID.empty() ) {
          Global_Token_init() ; 
          GetTokenAndType( gToken ) ; 
          tokentype = gToken.tokentype ;  
        } 
        
        
        else {
          Global_Token_init() ; 
          
          
          gToken.tokenID = CopyString( gPeekToken.tokenID ) ;
          gToken.tokenLine = gPeekToken.tokenLine ;
          gToken.tokenColumn = gPeekToken.tokenColumn ;
          gToken.tokentype = gPeekToken.tokentype ;
          for ( int i = 0 ; i < gPeekToken.token_char_string.size() ; i++ ) {
            gToken.token_char_string.push_back( gPeekToken.token_char_string[i] ) ;
          } 
          
          
          Global_Peek_Token_init() ;
          
          
          tokentype = gToken.tokentype ;
        } 


        if ( tokentype == RP ) { 
           
          
          inputString.push_back( gToken ) ;
          
          
         
          v.valueType = expValue.valueType ; 
          v.value = expValue.value ; 

          
          
          
          return true ;
        } 
        
        
        else { 

          
          if ( tokentype == NONE ) {
            mError.isError1 = true ;
            mError.errorToken = gToken.tokenID ;
            mError.errorline = gToken.tokenLine ;
            return false ;
          } 
          
          
          else {
            mError.isError2 = true ;
            mError.errorToken = gToken.tokenID ;
            mError.errorline = gToken.tokenLine ;
            return false ;
          } 
        } 
      } 

      else 
        return false ;
    } 
    
    
    
    else if ( tokentype == NONE ) {
      mError.isError1 = true ;
      mError.errorToken = gToken.tokenID ;
      mError.errorline = gToken.tokenLine ;
      return false ;
    } 

    
    else {
      mError.isError2 = true ;
      mError.errorToken = gToken.tokenID ;
      mError.errorline = gToken.tokenLine ;
      return false ;
    } 
  } 
  
  bool Not_ID_StartTerm( vector<TOKEN> &inputString, VALUERESULT &v ) {
    bool isCorrect = false ;
    TOKENTYPE tokentype ; 
    VALUERESULT factor1Value ;
    VALUERESULT factor2Value ;
    bool isFloat = false ;
    int temp1 = 0 ;
    int temp2 = 0 ;

    if ( Not_ID_StartFactor( inputString, factor1Value ) ) { 

      
      
      if ( factor1Value.valueType == value_float ) isFloat = true ;
      

      do {
        
        
        
        if ( gPeekToken.tokenID.empty() ) {
          
          
          PeekToken( gPeekToken ) ;
          
          
          tokentype = gPeekToken.tokentype ;  
        } 
        
        
        else {
          
          
          tokentype = gPeekToken.tokentype ;
          
        } 
        
        
        if ( tokentype == ISEOF || tokentype == STATE_END || ( tokentype != MUL && tokentype != DIV ) ) {
          isCorrect = true ;

          
          v.value = factor1Value.value ;
          
          
          if ( isFloat ) v.valueType = value_float ;
          
          
          else v.valueType = value_int ;
          
          
          return true ;
        } 
        
        
    
        
        if ( gPeekToken.tokenID.empty() ) {
          Global_Token_init() ; 
          GetTokenAndType( gToken ) ; 
          tokentype = gToken.tokentype ;  
        } 
        
        
        else {
          Global_Token_init() ; 
          
          
          gToken.tokenID = CopyString( gPeekToken.tokenID ) ;
          gToken.tokenLine = gPeekToken.tokenLine ;
          gToken.tokenColumn = gPeekToken.tokenColumn ;
          gToken.tokentype = gPeekToken.tokentype ;
          for ( int i = 0 ; i < gPeekToken.token_char_string.size() ; i++ ) {
            gToken.token_char_string.push_back( gPeekToken.token_char_string[i] ) ;
          } 
          
          
          Global_Peek_Token_init() ;
          
          
          tokentype = gToken.tokentype ;
        } 
        
        
        inputString.push_back( gToken ) ;
        

        if ( Factor( inputString, factor2Value ) ) { 
          isCorrect = true ;
        } 

        else { 
          isCorrect = false ;
          return false ; 
        } 


        if ( tokentype == MUL ) {
          
          if ( factor2Value.valueType == value_float ) isFloat = true ;
          
          factor1Value.value = factor1Value.value * factor2Value.value ;
        } 

        
        else { 

          
          if ( factor2Value.value == 0 ) {
            
            mError.isError5 = true ; 
            
            
            mError.errorline = inputString[inputString.size()-1].tokenLine ;
            return false ;
          } 

          
          else {
            
            if ( factor2Value.valueType == value_float ) isFloat = true ;
            
            
            if ( !isFloat ) {
              temp1 = factor1Value.value ; 
              temp2 = factor2Value.value ; 
              
              
              if ( temp1 % temp2 != 0 ) {
                isFloat = true ;
              } 
            } 
            
            
            factor1Value.value = factor1Value.value / factor2Value.value ;
          } 
        } 
        
        

      } while ( isCorrect ) ;
    } 

    
    return false ;
  } 
  
  bool Not_ID_StartArithExp( vector<TOKEN> &inputString, VALUERESULT &v ) {
    bool isCorrect = false ;
    TOKENTYPE tokentype ; 
    VALUERESULT term1Value ;
    VALUERESULT term2Value ;
    bool isFloat = false ;

    if ( Not_ID_StartTerm( inputString, term1Value ) ) { 

      if ( term1Value.valueType == value_float ) isFloat = true ; 
      

      do {

        if ( gPeekToken.tokenID.empty() ) {

          PeekToken( gPeekToken ) ;

          tokentype = gPeekToken.tokentype ;  
        } 
        
        
        else {
          
          
          tokentype = gPeekToken.tokentype ;
          
        } 
        
        
        if ( tokentype == ISEOF || tokentype == STATE_END || ( tokentype != ADD && tokentype != MINUS ) ) {
          isCorrect = true ;

          
          v.value = term1Value.value ;
          
          
          if ( isFloat ) v.valueType = value_float ;
          
          
          else v.valueType = value_int ;

          
          return true ;
        } 

        
    
        
        if ( gPeekToken.tokenID.empty() ) {
          Global_Token_init() ; 
          GetTokenAndType( gToken ) ; 
          tokentype = gToken.tokentype ;  
        } 
        
        
        else {
          Global_Token_init() ; 
          
          
          gToken.tokenID = CopyString( gPeekToken.tokenID ) ;
          gToken.tokenLine = gPeekToken.tokenLine ;
          gToken.tokenColumn = gPeekToken.tokenColumn ;
          gToken.tokentype = gPeekToken.tokentype ;
          for ( int i = 0 ; i < gPeekToken.token_char_string.size() ; i++ ) {
            gToken.token_char_string.push_back( gPeekToken.token_char_string[i] ) ;
          } 
          
          
          Global_Peek_Token_init() ;
          
          
          tokentype = gToken.tokentype ;
        } 
        
        
        inputString.push_back( gToken ) ;
        

        if ( Term( inputString, term2Value ) ) { 
          isCorrect = true ;
        } 

        else { 
          isCorrect = false ;
          return false ; 
        } 
        

           
        
        if ( tokentype == ADD ) {
          
          if ( term2Value.valueType == value_float ) isFloat = true ;
          
          term1Value.value = term1Value.value + term2Value.value ;
        } 

        
        else { 
          
          if ( term2Value.valueType == value_float ) isFloat = true ;
          
          term1Value.value = term1Value.value - term2Value.value ;
        } 
      } while ( isCorrect ) ;
    } 

    
    return false ;

  } 
  
   

  bool Not_ID_StartArithExpOrBexp( vector<TOKEN> &inputString, VALUERESULT &v ) {
    TOKENTYPE tokentype ; 
    VALUERESULT exp1Value ;
    VALUERESULT exp2Value ;
    double larger = 0.0  ;  
    double smaller = 0.0 ; 
    double t_TOLERANCE = 0.0001 ;
    bool isInTolerance = false ;
	  
    if ( Not_ID_StartArithExp( inputString, exp1Value ) ) { 
      
      
      
      v.value = exp1Value.value ;
      v.valueType = exp1Value.valueType ;
      
      
      
        
      
      if ( gPeekToken.tokenID.empty() ) {
        
        
        PeekToken( gPeekToken ) ;
        
        
        tokentype = gPeekToken.tokentype ; 
      } 

      
      else {
        
        tokentype = gPeekToken.tokentype ;
      } 
  

      
      if ( tokentype == EQ || tokentype == NEQ || tokentype == GT 
           || tokentype == LT || tokentype == GE || tokentype == LE ) {
               
        
    
        
        if ( gPeekToken.tokenID.empty() ) {
          Global_Token_init() ; 
          GetTokenAndType( gToken ) ; 
          tokentype = gToken.tokentype ;  
        } 
        
        
        else {
          Global_Token_init() ; 
          
          
          gToken.tokenID = CopyString( gPeekToken.tokenID ) ;
          gToken.tokenLine = gPeekToken.tokenLine ;
          gToken.tokenColumn = gPeekToken.tokenColumn ;
          gToken.tokentype = gPeekToken.tokentype ;
          for ( int i = 0 ; i < gPeekToken.token_char_string.size() ; i++ ) {
            gToken.token_char_string.push_back( gPeekToken.token_char_string[i] ) ;
          } 
          
          
          Global_Peek_Token_init() ;
          
          
          tokentype = gToken.tokentype ;
        } 
        
        
        inputString.push_back( gToken ) ;


        if ( ArithExp( inputString, exp2Value ) ) { 
        
          
          v.valueType = value_bool ;
          v.value = 0.0 ;

          


          
          if ( exp1Value.value > exp2Value.value ) {
            larger = exp1Value.value  ;
            smaller = exp2Value.value ;
          } 

          else {
            larger = exp2Value.value ;
            smaller = exp1Value.value ;
          } 

          
          if ( larger - smaller  <= t_TOLERANCE ) {
            isInTolerance = true ;
          } 

          
          if ( !isInTolerance ) {

            
            if ( tokentype == EQ ) {
              if ( exp1Value.value == exp2Value.value ) v.value_bool_ans = true;
              else v.value_bool_ans = false ;
            } 

            
            else if ( tokentype == NEQ ) {
              if ( exp1Value.value != exp2Value.value ) v.value_bool_ans = true;
              else v.value_bool_ans = false ;
            } 

            
            else if ( tokentype == GT ) {
              if ( exp1Value.value > exp2Value.value ) v.value_bool_ans = true ;
              else v.value_bool_ans = false ;
            } 

            
            else if ( tokentype == LT ) {
              if ( exp1Value.value < exp2Value.value ) v.value_bool_ans = true ;
              else v.value_bool_ans = false ;
            } 

            
            else if ( tokentype == GE ) {
              if ( exp1Value.value >= exp2Value.value ) v.value_bool_ans = true ;
              else v.value_bool_ans = false ;
            } 

            
            else if( tokentype == LE ) {
              if ( exp1Value.value <= exp2Value.value ) v.value_bool_ans = true ;
              else v.value_bool_ans = false ;
            } 
          } 

          
          else {
            if ( tokentype == EQ || tokentype == GE || tokentype == LE ) {
              v.value_bool_ans = true ;
            } 

            else {
              v.value_bool_ans = false ;
            } 
          } 

          
          

          
          return true ;
        } 

        else {  
          return false ;
        } 
      } 

      else 
        return true ;
    } 

    
    return false ;

  } 
  
  
  
  bool IDlessArithExpOrBexp( vector<TOKEN> &inputString, VALUERESULT &v, string id_name ) {
    bool isCorrect = false ;
    TOKENTYPE tokentype ; 
    VALUERESULT termValue ;
    VALUERESULT factorValue ;
    VALUERESULT expValue ;
    VALUERESULT inputValue ;
    bool isFloat = false ;
    double t_TOLERANCE = 0.0001;
    double larger = 0.0 ;
    double smaller = 0.0 ;
    double isInTolerance = false ;
    bool isBreak = false ;
    int temp1 = 0 ;
    int temp2 = 0 ;

    int symbolIndex = -1 ;
    
    
        
    
    if ( gPeekToken.tokenID.empty() ) {
      
      
      PeekToken( gPeekToken ) ;
      
      
      tokentype = gPeekToken.tokentype ; 
    } 

    
    else {
      
      tokentype = gPeekToken.tokentype ;
    } 

    
    if ( tokentype == ADD || tokentype == MINUS || tokentype == MUL || tokentype == DIV ) {
      
      
      
      symbolIndex = IsSymbolExist( id_name ) ; 

      
      
      if ( symbolIndex != -1 ) {
      
        inputValue.value = gSymbolList[symbolIndex].symbolValue;
        inputValue.valueType = gSymbolList[symbolIndex].symbolType ;
      } 

      
      else {
        
        
        
        if ( gPeekToken.tokenID.empty() ) {
          Global_Token_init() ; 
          GetTokenAndType( gToken ) ; 
          tokentype = gToken.tokentype ;  
        } 
        
        
        else {
          Global_Token_init() ; 
          
          
          gToken.tokenID = CopyString( gPeekToken.tokenID ) ;
          gToken.tokenLine = gPeekToken.tokenLine ;
          gToken.tokenColumn = gPeekToken.tokenColumn ;
          gToken.tokentype = gPeekToken.tokentype ;
          for ( int i = 0 ; i < gPeekToken.token_char_string.size() ; i++ ) {
            gToken.token_char_string.push_back( gPeekToken.token_char_string[i] ) ;
          } 
          
          
          Global_Peek_Token_init() ;
          
          
          tokentype = gToken.tokentype ;
        } 

        
        mError.isError3 = true ;
        mError.errorToken = inputString[inputString.size()-1].tokenID ;    
        
        mError.errorline = gToken.tokenLine ;  
        return false ;
      } 
     
      
      
      
      if ( inputValue.valueType == value_float ) isFloat = true ;

      do {
        
        
        
        
        if ( gPeekToken.tokenID.empty() ) {
          
          
          PeekToken( gPeekToken ) ;
          
          
          tokentype = gPeekToken.tokentype ; 
        } 
    
        
        else {
          
          tokentype = gPeekToken.tokentype ;
        } 
        
        
        if ( tokentype == ISEOF || tokentype == STATE_END ||
          ( tokentype != ADD && tokentype != MINUS && tokentype != MUL && tokentype != DIV ) ) {

          isCorrect = true ;

          
          v.value = inputValue.value ;
          
          
          if ( isFloat ) v.valueType = value_float ;
          
          
          else v.valueType = value_int ;
          
          
          
          isBreak = true ;
        } 

        if ( !isBreak ) {
          
          
          
    
          
          if ( gPeekToken.tokenID.empty() ) {
            Global_Token_init() ; 
            GetTokenAndType( gToken ) ; 
            tokentype = gToken.tokentype ;  
          } 
          
          
          else {
            Global_Token_init() ; 
            
            
            gToken.tokenID = CopyString( gPeekToken.tokenID ) ;
            gToken.tokenLine = gPeekToken.tokenLine ;
            gToken.tokenColumn = gPeekToken.tokenColumn ;
            gToken.tokentype = gPeekToken.tokentype ;
            for ( int i = 0 ; i < gPeekToken.token_char_string.size() ; i++ ) {
              gToken.token_char_string.push_back( gPeekToken.token_char_string[i] ) ;
            } 
            
            
            Global_Peek_Token_init() ;
            
            
            tokentype = gToken.tokentype ;
          } 
          
          
          
          if ( tokentype == ADD || tokentype == MINUS ) { 
          
            
            inputString.push_back( gToken ) ;
            
            if ( Term( inputString, termValue ) ) { 
              isCorrect = true ;
            } 

            else { 
              isCorrect = false ;
              return false ;
            } 
                  
            
            if ( tokentype == ADD ) {
              
              if ( termValue.valueType == value_float ) isFloat = true ;
              
              inputValue.value = inputValue.value + termValue.value ;
            } 
  
            
            else { 
              
              if ( termValue.valueType == value_float ) isFloat = true ;
              
              inputValue.value = inputValue.value - termValue.value ;
            } 
                
          } 
          
          
          else if ( tokentype == MUL || tokentype == DIV ) {
            
            
            inputString.push_back( gToken ) ;
            
            if ( Factor( inputString, factorValue ) ) { 
              isCorrect = true ;
            } 

            else {   
              isCorrect = false ;
              return false ;
            } 
                                    
            
            if ( tokentype == MUL ) { 
              
              if ( factorValue.valueType == value_float ) isFloat = true ;
              
              inputValue.value = inputValue.value * factorValue.value ;
            } 

            
            else { 

              
              if ( factorValue.value == 0 ) {
                mError.isError5 = true ;
                mError.errorline = inputString[inputString.size()-1].tokenLine ;
                return false ;
              } 

              
              else {
                
                if ( factorValue.valueType == value_float ) isFloat = true ;
                
                
                if ( !isFloat ) {
                  temp1 = inputValue.value ; 
                  temp2 = factorValue.value ; 
                  
                  
                  if ( temp1 % temp2 != 0 ) {
                    isFloat = true ;
                  } 
                } 
                
                
                inputValue.value = inputValue.value / factorValue.value ;	
              } 
            } 
          } 
        } 
      } while ( isCorrect && !isBreak ) ;  
      
      
        
      
      if ( gPeekToken.tokenID.empty() ) {
        
        
        PeekToken( gPeekToken ) ;
        
        
        tokentype = gPeekToken.tokentype ; 
      } 
  
      
      else {
        
        tokentype = gPeekToken.tokentype ;
      } 

      
      if ( tokentype == EQ || tokentype == NEQ || tokentype == GT 
            || tokentype == LT || tokentype == GE || tokentype == LE ) {
        
        
        
    
        
        if ( gPeekToken.tokenID.empty() ) {
          Global_Token_init() ; 
          GetTokenAndType( gToken ) ; 
          tokentype = gToken.tokentype ;  
        } 
        
        
        else {
          Global_Token_init() ; 
          
          
          gToken.tokenID = CopyString( gPeekToken.tokenID ) ;
          gToken.tokenLine = gPeekToken.tokenLine ;
          gToken.tokenColumn = gPeekToken.tokenColumn ;
          gToken.tokentype = gPeekToken.tokentype ;
          for ( int i = 0 ; i < gPeekToken.token_char_string.size() ; i++ ) {
            gToken.token_char_string.push_back( gPeekToken.token_char_string[i] ) ;
          } 
          
          
          Global_Peek_Token_init() ;
          
          
          tokentype = gToken.tokentype ;
        } 

        
        inputString.push_back( gToken ) ;
        

        if ( ArithExp( inputString, expValue ) ) { 

          
          v.valueType = value_bool ;
          v.value = 0.0 ;
            
          
          if ( inputValue.value > expValue.value ) {
            larger = inputValue.value ;
            smaller = expValue.value ;
          } 

          else {
            larger = expValue.value ;
            smaller = inputValue.value ;
          } 

          if ( larger - smaller <= t_TOLERANCE ) {
            isInTolerance = true ;
          } 


          
          if ( !isInTolerance ) {

            
            if ( tokentype == EQ ) {
              if ( inputValue.value == expValue.value ) v.value_bool_ans = true ;
              else v.value_bool_ans = false ;
            } 

            
            else if ( tokentype == NEQ ) {
              if ( inputValue.value != expValue.value ) v.value_bool_ans = true ;
              else v.value_bool_ans = false ;
            } 

            
            else if ( tokentype == GT ) {
              if ( inputValue.value > expValue.value ) v.value_bool_ans = true ;
              else v.value_bool_ans = false ;
            } 

            
            else if ( tokentype == LT ) {
              if ( inputValue.value < expValue.value ) v.value_bool_ans = true ;
              else v.value_bool_ans = false ;
            } 

            
            else if ( tokentype == GE ) { 
              if ( inputValue.value >= expValue.value ) v.value_bool_ans = true ;
              else v.value_bool_ans = false ;
            } 

            
            else if ( tokentype == LE ) {
              if ( inputValue.value <= expValue.value ) v.value_bool_ans = true ;	
              else v.value_bool_ans = false ;				
            } 
          } 

          
          else {
            
            if ( tokentype == EQ || tokentype == GE || tokentype == LE ) {
              v.value_bool_ans = true ;
            } 

            else {
              v.value_bool_ans = false ;
            } 
          } 
                         
          
          return true ;
        } 

        else {  
          return false ;
        } 
      } 
      
      
      
      else 
        return true ;
     } 

    
    
    else if ( tokentype == EQ || tokentype == NEQ || tokentype == GT 
              || tokentype == LT || tokentype == GE || tokentype == LE ) {
      
      
      
    
      
      if ( gPeekToken.tokenID.empty() ) {
        Global_Token_init() ; 
        GetTokenAndType( gToken ) ; 
        tokentype = gToken.tokentype ;  
      } 
      
      
      else {
        Global_Token_init() ; 
        
        
        gToken.tokenID = CopyString( gPeekToken.tokenID ) ;
        gToken.tokenLine = gPeekToken.tokenLine ;
        gToken.tokenColumn = gPeekToken.tokenColumn ;
        gToken.tokentype = gPeekToken.tokentype ;
        for ( int i = 0 ; i < gPeekToken.token_char_string.size() ; i++ ) {
          gToken.token_char_string.push_back( gPeekToken.token_char_string[i] ) ;
        } 
        
        
        Global_Peek_Token_init() ;
        
        
        tokentype = gToken.tokentype ;
      } 

      
      inputString.push_back( gToken ) ;

      
      symbolIndex = IsSymbolExist( id_name ) ; 

      
      if ( symbolIndex != -1 ) {
      
        inputValue.value = gSymbolList[symbolIndex].symbolValue;
        inputValue.valueType = gSymbolList[symbolIndex].symbolType ;
      } 

      
      else {

        
        mError.isError3 = true ;
        mError.errorToken = inputString[inputString.size()-2].tokenID ;    
        
        mError.errorline = gToken.tokenLine ;  
        return false ;
      } 
      
      


      if ( ArithExp( inputString, expValue ) ) { 

        
        v.value = 0.0 ;
        v.valueType = value_bool ;
        
          
        
        if ( inputValue.value > expValue.value ) {
          larger = inputValue.value ;
          smaller = expValue.value ;
        } 

        else {
          larger = expValue.value ;
          smaller = inputValue.value  ;
        } 

        if ( larger - smaller <= t_TOLERANCE ) {
          isInTolerance = true ;
        } 

        
        if ( tokentype == EQ ) {  

          
          if ( isInTolerance ) {
            
            v.value_bool_ans = true ;			
          } 

          
          else {
            if ( inputValue.value == expValue.value ) v.value_bool_ans = true ;
            else v.value_bool_ans = false ;
          } 
        } 

        
        else if ( tokentype == NEQ ) {

          
          if ( isInTolerance ) {
            
            v.value_bool_ans = false ;
          } 

	        
          else {
            if ( inputValue.value != expValue.value ) v.value_bool_ans = true ;
            else v.value_bool_ans = false ;
          } 
        } 

        
        else if ( tokentype == GT ) {

          
          if ( isInTolerance ) {
            
            v.value_bool_ans = false ;
          } 

          
          else {
            if ( inputValue.value > expValue.value ) v.value_bool_ans = true ;
            else v.value_bool_ans = false ;
          } 
        } 

        
        else if ( tokentype == LT ) {

          
          if ( isInTolerance ) {
            
            v.value_bool_ans = false ;
          } 

          
          else {
            if ( inputValue.value < expValue.value ) v.value_bool_ans = true ;
            else v.value_bool_ans = false ;
          } 
        } 

        
        else if ( tokentype == GE ) {

          
          if ( isInTolerance ) {
            
            v.value_bool_ans = true ;
          } 

          
          else {
            if ( inputValue.value >= expValue.value ) v.value_bool_ans = true ;
            else v.value_bool_ans = false ;
          } 
        } 

        
        else if ( tokentype == LE ) {

          
          if ( isInTolerance ) { 
            
            v.value_bool_ans = true ;		
          } 

          
          else {
            if ( inputValue.value <= expValue.value ) v.value_bool_ans = true ;
            else v.value_bool_ans = false ;
          } 
        } 
  
        
        
        
        return true ;
      } 

      else { 
        return false ;
      } 
    } 

    else {  
    
      
      symbolIndex = IsSymbolExist( id_name ) ; 

      
      if ( symbolIndex != -1 ) {
      
        v.value = gSymbolList[symbolIndex].symbolValue;
        v.valueType = gSymbolList[symbolIndex].symbolType ;
      } 

      
      else {
        v.value = 0.0 ;
        v.valueType = value_not_define ;
      } 
      
      
      
      return true ;
    } 
  } 
  
  /*
  <Command> ::= IDENT ( ':=' <ArithExp> | <IDlessArithExpOrBexp> ) ';' 
    | <NOT_IDStartArithExpOrBexp> ';' 
    | QUIT
  */

  bool Command( vector<TOKEN> &inputString, VALUERESULT &v, bool &isQuit ) {
    TOKENTYPE tokentype ; 
    SYMBOL symbol ;
    double tokenValue = 0.0 ;
    VALUERESULT expValue ;
    VALUERESULT inputValue ;
    string id_name = "" ;
    int symbolIndex = -1 ;
    

    if ( gPeekToken.tokenID.empty() ) {
      
      PeekToken( gPeekToken ) ;
      
      
      tokentype = gPeekToken.tokentype ; 
    } 

    
    else {
      
      tokentype = gPeekToken.tokentype ;
    } 
        
    
    
    if ( IsEnd( gPeekToken.tokenID ) ) { 
      isQuit = true ;  
      return true ; 
    } 

    
    if ( tokentype == IDENT ) {
      
      
      if ( gPeekToken.tokenID.empty() ) {
        Global_Token_init() ; 
        GetTokenAndType( gToken ) ; 
        tokentype = gToken.tokentype ;  
      } 
      
      
      else {
        Global_Token_init() ; 
        
        
        gToken.tokenID = CopyString( gPeekToken.tokenID ) ;
        gToken.tokenLine = gPeekToken.tokenLine ;
        gToken.tokenColumn = gPeekToken.tokenColumn ;
        gToken.tokentype = gPeekToken.tokentype ;
        for ( int i = 0 ; i < gPeekToken.token_char_string.size() ; i++ ) {
          gToken.token_char_string.push_back( gPeekToken.token_char_string[i] ) ;
        } 
        
        
        Global_Peek_Token_init() ;
        
        
        tokentype = gToken.tokentype ;
      } 

      
      inputString.push_back( gToken ) ;

      
      
      id_name = CopyString( gToken.tokenID ) ; 
      
      
      
        
      
      if ( gPeekToken.tokenID.empty() ) {
        
        
        PeekToken( gPeekToken ) ;
        
        
        tokentype = gPeekToken.tokentype ; 
      } 
  
      
      else {
        
        tokentype = gPeekToken.tokentype ;
      } 

      
      if ( tokentype == DEFINE ) { 
                    
        
        if ( gPeekToken.tokenID.empty() ) {
          Global_Token_init() ; 
          GetTokenAndType( gToken ) ; 
          tokentype = gToken.tokentype ;  
        } 
        
        
        else {
          Global_Token_init() ; 
          
          
          gToken.tokenID = CopyString( gPeekToken.tokenID ) ;
          gToken.tokenLine = gPeekToken.tokenLine ;
          gToken.tokenColumn = gPeekToken.tokenColumn ;
          gToken.tokentype = gPeekToken.tokentype ;
          for ( int i = 0 ; i < gPeekToken.token_char_string.size() ; i++ ) {
            gToken.token_char_string.push_back( gPeekToken.token_char_string[i] ) ;
          } 
          
          
          Global_Peek_Token_init() ;
          
          
          tokentype = gToken.tokentype ;
        } 
  
        
        inputString.push_back( gToken ) ;

        if ( ArithExp( inputString, expValue ) ) { 
                  	
          
          if ( gPeekToken.tokenID.empty() ) {
            Global_Token_init() ; 
            GetTokenAndType( gToken ) ; 
            tokentype = gToken.tokentype ;  
          } 
          
          
          else {
            Global_Token_init() ; 
            
            
            gToken.tokenID = CopyString( gPeekToken.tokenID ) ;
            gToken.tokenLine = gPeekToken.tokenLine ;
            gToken.tokenColumn = gPeekToken.tokenColumn ;
            gToken.tokentype = gPeekToken.tokentype ;
            for ( int i = 0 ; i < gPeekToken.token_char_string.size() ; i++ ) {
              gToken.token_char_string.push_back( gPeekToken.token_char_string[i] ) ;
            } 
            
            
            Global_Peek_Token_init() ;
            
            
            tokentype = gToken.tokentype ;
          } 
             

          if ( tokentype == STATE_END ) { 

            
            inputString.push_back( gToken ) ;
            
            

            symbolIndex = IsSymbolExist( id_name ) ;  

            
            if ( symbolIndex == -1 ) {
              
              symbol.symbolName = CopyString( id_name ) ;
              symbol.symbolValue = expValue.value ;
              symbol.symbolType = expValue.valueType ;
              gSymbolList.push_back( symbol ) ;

              
              v.valueType = expValue.valueType ;
              v.value = expValue.value ;
            } 
  
            else {
              
              gSymbolList[symbolIndex].symbolValue = expValue.value ;
              gSymbolList[symbolIndex].symbolType = expValue.valueType ;
  
              
              v.valueType = expValue.valueType ;
              v.value = expValue.value ;
            } 
            
            
            return true ;
            
          } 
          
          
          
          else {
            
            if ( tokentype == NONE ) {
              mError.isError1 = true ;
              mError.errorToken = gToken.tokenID ;    
              mError.errorline =  gToken.tokenLine ;  
              return false ;
            } 

            
            else {
              mError.isError2 = true ;
              mError.errorToken = gToken.tokenID ; 
              mError.errorline =  gToken.tokenLine ; 
              return false ;
            } 
          } 
        } 

        else { 
          return false ;
        } 
      } 

      
      else {
        
        if ( IDlessArithExpOrBexp( inputString, expValue, id_name ) ) {
          
          
          if ( gPeekToken.tokenID.empty() ) {
            Global_Token_init() ; 
            GetTokenAndType( gToken ) ; 
            tokentype = gToken.tokentype ;  
          } 

          
          else {
            Global_Token_init() ; 
            
            
            gToken.tokenID = CopyString( gPeekToken.tokenID ) ;
            gToken.tokenLine = gPeekToken.tokenLine ;
            gToken.tokenColumn = gPeekToken.tokenColumn ;
            gToken.tokentype = gPeekToken.tokentype ;
            for ( int i = 0 ; i < gPeekToken.token_char_string.size() ; i++ ) {
              gToken.token_char_string.push_back( gPeekToken.token_char_string[i] ) ;
            } 
            
            
            Global_Peek_Token_init() ;
            
            
            tokentype = gToken.tokentype ;
          } 
    
          
          if ( tokentype == STATE_END ) {  

            
            inputString.push_back( gToken ) ;

            
            
            if ( expValue.valueType == value_not_define ) {

              
              mError.isError3 = true ;
              mError.errorToken = inputString[inputString.size()-2].tokenID ;
              
              mError.errorline = gToken.tokenLine ;
              return false ;
            } 
            
            
            else {
              v.value = expValue.value ;
              v.valueType = expValue.valueType ;
              v.value_bool_ans = expValue.value_bool_ans ; 
            } 
            
            
            return true ;
            
          } 
          
          
          else {
            
            
            if ( tokentype == NONE ) {
              mError.isError1 = true ;
              mError.errorToken = gToken.tokenID ;  
              mError.errorline =  gToken.tokenLine ; 
              return false ;
            } 

            
            else {
              mError.isError2 = true ;
              mError.errorToken = gToken.tokenID ;  
              mError.errorline =  gToken.tokenLine ; 
              return false ;
            } 
          } 
        } 

        else { 
          return false ;
        } 
      } 
    } 

    
    else {			
      
      if ( Not_ID_StartArithExpOrBexp( inputString, expValue ) ) {
        
        
        
        if ( gPeekToken.tokenID.empty() ) {
          Global_Token_init() ; 
          GetTokenAndType( gToken ) ; 
          tokentype = gToken.tokentype ;  
        } 
        
        
        else {
          Global_Token_init() ; 
          
          
          gToken.tokenID = CopyString( gPeekToken.tokenID ) ;
          gToken.tokenLine = gPeekToken.tokenLine ;
          gToken.tokenColumn = gPeekToken.tokenColumn ;
          gToken.tokentype = gPeekToken.tokentype ;
          for ( int i = 0 ; i < gPeekToken.token_char_string.size() ; i++ ) {
            gToken.token_char_string.push_back( gPeekToken.token_char_string[i] ) ;
          } 
          
          
          Global_Peek_Token_init() ;
          
          
          tokentype = gToken.tokentype ;
        } 

        if ( tokentype == STATE_END ) { 
          
          inputString.push_back( gToken ) ;
          
          
          v.valueType = expValue.valueType ;
          v.value = expValue.value ;
          v.value_bool_ans = expValue.value_bool_ans ; 
          
          
          
          return true ;
        } 
       
        
        else {
          
          if ( tokentype == NONE ) {
            mError.isError1 = true ;
            mError.errorToken = gToken.tokenID ; 
            mError.errorline =  gToken.tokenLine ; 
            return false ;
          } 

          
          else {
            mError.isError2 = true ;
            mError.errorToken = gToken.tokenID ; 
            mError.errorline =  gToken.tokenLine ; 
            return false ;
          } 
        } 
      } 

      else { 
        return false ;
      } 
    } 
  } 

}; 


int main() {
  
  int testNum ; 
  vector<TOKEN> inputString ; 
  TOKENTYPE tokentype ;
  SYNTAX s ;
  VALUERESULT v ;
  bool isQuit = false ;
  int errorLine = 0 ;


  s.Error_init() ;
  Global_Peek_Token_init() ;
  Global_Token_init() ;

  cin >> testNum ;
  cout << "Program starts..." << endl ;
  
  while ( !isQuit ) {
    
    
    if ( s.Command( inputString, v, isQuit ) ) {
      
      
      if ( isQuit ) {
        cout << "> Program exits..." << endl ;
        return 0 ; 
      } 

      
      else {

        
        if ( v.valueType == value_bool ) {
          if ( v.value_bool_ans ) cout << "> true" << endl ;
          else cout << "> false" << endl ;
        } 

        else if ( v.valueType == value_int ) {
          printf( "> %.f\n",  v.value ) ;
          
        } 

        else if ( v.valueType == value_float ) {  
          printf( "> %1.3f\n",  v.value ) ;  
        } 
      } 
      
      
      Global_Token_init() ;  
    } 

    else {
      
      if ( s.mError.isError1 ) {
                
        while ( gLine == s.mError.errorline ) {
          Get_next_ch( gnextchar )  ;
        } 

        cout << "> Unrecognized token with first char : '" << s.mError.errorToken << "\'" << endl ;
      } 

      else if ( s.mError.isError2 ) {
        while ( gLine == s.mError.errorline ) {          
          Get_next_ch( gnextchar )  ;
        } 

        cout << "> Unexpected token : '" << s.mError.errorToken << "\'"  << endl ;
      } 
      
      
      else if ( s.mError.isError3 ) {
        while ( gLine == s.mError.errorline ) {
          Get_next_ch( gnextchar )  ;
        } 
        
        cout << "> Undefined identifier : '" << s.mError.errorToken << "\'" << endl ;
      } 
      
      
      else if ( s.mError.isError5 ) {
        while ( gLine == s.mError.errorline ) {         
          Get_next_ch( gnextchar )  ;
        } 
        cout << "> Error" << endl ;
      } 

      else { 
        cout << "EOF error" << endl ;
      } 
      
      Global_Token_init() ;
      gcurchar = gnextchar ; 
    } 
    
    
    s.Error_init() ; 

    
    v.value = 0.0 ;
    v.value_bool_ans = false ;
    v.valueType = value_not_define ;
    
    
    inputString.clear() ;
    if ( gisEOF )  return 0 ; 
    
  } 
  
  return 0;
} 
