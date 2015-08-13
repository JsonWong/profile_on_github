package Table;

public class Adjcell {
	private String _CellID;
	private String _AdjCellID;
	private String _CellLac;
	private String _AdjCellLac;
	
	private int CellID;
	private int AdjCellID;
	private int CellLac;
	private int AdjCellLac;
	
	public Adjcell(String _CellID, String _AdjCellID, String _CellLac, String _AdjCellLac){
		this._CellID = _CellID;
		this._AdjCellID = _AdjCellID;
		this._CellLac = _CellLac;
		this._AdjCellLac = _AdjCellLac;
		
		this.CellID = 0;
		this.AdjCellID = 0;
		this.CellLac = 0;
		this.AdjCellLac = 0;
	}
	
	public boolean checkRow(){
		try{
			CellID = Integer.parseInt(_CellID);
			AdjCellID = Integer.parseInt(_AdjCellID);
			CellLac = Integer.parseInt(_CellLac);
			AdjCellLac = Integer.parseInt(_AdjCellLac);
			if (CellID < 0 || AdjCellID < 0 || CellLac < 0 || AdjCellLac < 0)
				return false;
			return true;
		} catch(NumberFormatException e){
			return false;
		}
	}
	
	public int getCellID() {
		return CellID;
	}

	public int getAdjCellID() {
		return AdjCellID;
	}

	public int getCellLac() {
		return CellLac;
	}

	public int getAdjCellLac() {
		return AdjCellLac;
	}
}
