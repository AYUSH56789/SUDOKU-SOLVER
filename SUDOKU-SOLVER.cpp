// IN THI PROJECT WE NEED TO GIVE ALL VALUE IN UNSOLVED PUZZLE AND WRITE '0' AT BLANK SPACE AND AFTER THAT OUR PROGRAM SOLVE THAT UNSOLVED PUZZLE WITH BACKTRACKING ALGORITHM AND RETURN SOLVED PUZZLE.
#include<iostream>
using namespace std;
class SudokuFrame{	
	int sudokuFrame[9][9]; 
	int editableFrame[9][9];
	public:SudokuFrame(){
		menu();
	}
	private:void menu(){		
		cout<<"\t\t\t\t =============================================\n";
		cout<<"\t\t\t\t            Welcome Sudoku Solver\n";
		cout<<"\t\t\t\t =============================================\n\n";
		cout<<"--> To start [Enter - 1]\n";	
		cout<<"--> ";
		int option;
	    cin>>option;
		cout<<"--> Input the puzzle by entering the values manually:\n";
		if(option==1) readFrameValues();
		else{
			while(true){
				cout<<"\nYou seem to have entered an invalid option. Try again.\n";
				cout<<"\t   --> ";
				cin>>option;
				if(option==1) readFrameValues();
				else continue;
				break;
			}
		}
	}
	private:void readFrameValues(){
		cout<<"--> ***Enter 0 if cell is empty.***\n\n";		
		int rowIter, colIter;
		for(rowIter=0; rowIter<9; rowIter++){
			for(colIter=0; colIter<9; colIter++){
				int enteredValue;
				cout<<"Enter value for cell["<<rowIter+1<<"]["<<colIter+1<<"] --> ";
				cin>>enteredValue;
				if(!(enteredValue>=0 && enteredValue<=9)){ 
					while(true){ 
						cout<<"Oops! You seem to have entered a wrong value! Try again.\n";
						cout<<"Enter value for cell ["<<rowIter+1<<"]["<<colIter+1<<"] --> ";
						cin>>enteredValue;
						if(enteredValue>=0 && enteredValue<=9) break;
					}
				}
				sudokuFrame[rowIter][colIter]=enteredValue;
				if(enteredValue==0) editableFrame[rowIter][colIter]=0;
				else editableFrame[rowIter][colIter]=1;
			}
			cout<<"-------\n"; 
		}
	}
	
	public:void setCellValue(int row, int col, int num){
		if(editableFrame[row][col]==0) sudokuFrame[row][col]=num;
	}
	public:int getCellValue(int row, int col){
		int cellValue=sudokuFrame[row][col];
		return cellValue;
	}
	public:int isEditable(int row, int col){
		return (editableFrame[row][col]-1)*(-1);
	}
	public:void clearFrameFrom(int row, int col){
		int jcount=0;
		int rowIter, colIter;
		for(rowIter=row; rowIter<9; rowIter++){			
			if(jcount==0) colIter=col;
			else colIter=0;
			for(; colIter<9; colIter++){
				if(editableFrame[rowIter][colIter]==0) sudokuFrame[rowIter][colIter]=0;
			}
			jcount++;
		}
	}
	public:void displayFrame(){

		cout<<"\033[0;36m++=====================================++";
		int rowIter, colIter;

		for(rowIter=0; rowIter<9; rowIter++){
			int cellIter=1;

			cout<<"\n\033[0;36m||";
			for(colIter=0; colIter<9; colIter++){
				if(cellIter==3){
					cout<<"\033[0m "<<sudokuFrame[rowIter][colIter]<<" ";
					cout<<"\033[0;36m||";
					cellIter=1;
				}
				else{
					cout<<"\033[0m "<<sudokuFrame[rowIter][colIter]<<"  ";
					cellIter++;	
				}
			}
			if(rowIter%3!=2) cout<<"\n\033[0;36m++-----------++-----------++-----------++";
			else cout<<"\n\033[0;36m++=====================================++";
		}
	}
};
class Possibilities{
	struct node{
		int value;
		struct node* next;
	};	
	typedef struct node* Node;
	Node head; 
	Node pos;
	public:Possibilities(){
		head=new struct node;
		head->next=NULL;
	}
	public:~Possibilities(){
		destroy();
	}
	public:void append(int number){
		Node temp=new struct node;
		temp->value=number;
		temp->next=NULL;
		pos=head;
		while(pos!=NULL){
			if(pos->next==NULL){
				pos->next=temp;
				break;
			}
			pos=pos->next;
		}
	}
	public:int operator[](int index){
		int count=0;
		pos=head->next;		
		while(pos!=NULL){
			if(count==index)
				return pos->value;
			pos=pos->next;
			count++;
		}
		return -1;
	}
	public:void print(){
		pos=head->next;
		while(pos!=NULL){
			cout<<pos->value<<",";
			pos=pos->next;
		}
		cout<<"\b";
	}
	
	public:int length(){
		pos=head->next;
		int count=0;
		while(pos!=NULL){
			count++;
			pos=pos->next;
		}		
		return count;
	}
	public:void copy(Possibilities possibilities){ 
		int oldLength=possibilities.length();
		int iter=0;		
		pos=head;
		for(iter=0; iter<oldLength; iter++){
			Node temp=new struct node;
			temp->next=NULL;
			temp->value=possibilities[iter];
			pos->next=temp;
			pos=pos->next;
		}
	}
	private:void destroy(){
		Node temp;
		pos=head;
		while(pos!=NULL){
			temp=pos;
			pos=pos->next;
			delete temp;
		}
	}
};
class SudokuSolver{	
	int recursiveCount;
	SudokuFrame frame; 	
	public:SudokuSolver(){
		recursiveCount=0;
		cout<<"\nCalculating possibilities...\n";
		cout<<"Backtracking across puzzle....\n";
		cout<<"Validating cells and values...\n\n";		
		solve();
		cout<<"QED. Your puzzle has been solved!\n\n";
		displayFrame();
		cout<<"\n\n";
	}
	private:bool cellValueValid(int row, int col, int currentValue){
		int rowIter, colIter;
		for(rowIter=0; rowIter<9; rowIter++){
			if(rowIter!=row){
				int comparingValue=frame.getCellValue(rowIter,col);
				if(comparingValue==currentValue) return false;
			}
		}
		for(colIter=0; colIter<9; colIter++){
			if(colIter!=col){
				int comparingValue=frame.getCellValue(row,colIter);
				if(comparingValue==currentValue) return false;
			}
		}
		if(ThreeByThreeGridValid(row,col,currentValue)==false) return false;
		return true;
	}
	private:bool ThreeByThreeGridValid(int row, int col, int currentValue){
		int rowStart=(row/3)*3;
		int rowEnd=(rowStart+2);
		int colStart=(col/3)*3;
		int colEnd=(colStart+2);
		int rowIter, colIter;
		for(rowIter=rowStart; rowIter<=rowEnd; rowIter++){
			for(colIter=colStart; colIter<=colEnd; colIter++){
				if(frame.getCellValue(rowIter,colIter)==currentValue) return false;
			}
		}
		return true;	
	}	
	private:Possibilities getCellPossibilities(int row, int col){
		int iter=0;
		Possibilities possibilities;
		for(iter=1; iter<=9; iter++){
			if(cellValueValid(row,col,iter)==true)
				possibilities.append(iter);
		}		return possibilities;
	}
	private:int singleCellSolve(int row, int col){		
		statsIncrement();
		if(frame.isEditable(row,col)==true){
			Possibilities possibilities;
			possibilities.copy(getCellPossibilities(row,col));
			int posLength=possibilities.length();
			int posIter=0, newRow=row, newCol=col;
			for(posIter=0; posIter<posLength; posIter++){ 
				int possibility=possibilities[posIter];
				frame.setCellValue(row,col,possibility);
				if(col<8) newCol=col+1; 
				else if(col==8){
					if(row==8) return 1; 
					newRow=row+1;
					newCol=0;
				}
				{
					if(singleCellSolve(newRow,newCol)==0){ 
						frame.clearFrameFrom(newRow,newCol);
					}
					else return 1;
				} 
			} 
			return 0;		
		} 
		else{
			int newRow=row, newCol=col;
			if(col<8) newCol=col+1;
			else if(col==8){
				if(row==8) return 1;
				newRow=row+1;
				newCol=0;
			}
			return singleCellSolve(newRow,newCol);
		} 
	}
	private:void solve(){
		int success=singleCellSolve(0,0);
	}
	private:void displayFrame(){
		frame.displayFrame();
	}
	private:void statsIncrement(){
		recursiveCount++;
	}
	public:void statsPrint(){
		cout<<"\nThe singleCellSolve() function was recursively called "<<recursiveCount<<" times.\n";
	}	
};
int main(){
	SudokuSolver ss;
	return 0;
}
