package Table;

public class MS_Cell_free {
	private String _CellID;
	private String _IMEI;
	
	private int CellID;
	private String IMEI;	

	public MS_Cell_free(String _CellID, String _IMEI){
		this._CellID = _CellID;
		this._IMEI = _IMEI;
		
		this.CellID = 0;
		this.IMEI = _IMEI;
	}
	
	public boolean checkRow(){
		try{
			CellID = Integer.parseInt(_CellID);
			if (CellID < 0 || IMEI == "")
				return false;
			return true;
		} catch(NumberFormatException e){
			return false;
		}
	}

	public int getCellID() {
		return CellID;
	}

	public String getIMEI() {
		return IMEI;
	}
}
