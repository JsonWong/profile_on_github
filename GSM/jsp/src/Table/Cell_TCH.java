package Table;

public class Cell_TCH {
	private String _CellID;
	private String _Freq;
	
	private int CellID;
	private int Freq;	

	public Cell_TCH(String _CellID, String _AdjCellID){
		this._CellID = _CellID;
		this._Freq = _Freq;
		
		this.CellID = 0;
		this.Freq = 0;
	}
	
	public boolean checkRow(){
		try{
			CellID = Integer.parseInt(_CellID);
			Freq = Integer.parseInt(_Freq);
			if (CellID < 0 || Freq < 0)
				return false;
			return true;
		} catch(NumberFormatException e){
			return false;
		}
	}
	
	public int getCellID() {
		return CellID;
	}

	public int getFreq() {
		return Freq;
	}	
}
