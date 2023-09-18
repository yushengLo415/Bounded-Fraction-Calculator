#ifndef MAINFORM
#define MAINFORM

#include "FileInput.h"
#include <map>

namespace CppCLRWinformsProjekt {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Zusammenfassung für MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
		public:
			MainForm(void);

		protected:
			~MainForm();

		private: 
			Button^ _import_button;
			TextBox^ _filePathTextBox;
			Label^ _contact_fraction_label;
			Label^ _unbounded_fraction_label;
			Label^ _threshold_label;
			TextBox^ _threshold_value_textBox;
			Label^ _contact_fraction_value_label;
			Label^ _unbounded_fraction_value_label;
			Panel^ _panel;
			Button^ _screen_shot_button;
			Label^ _file_name_title_label;
			Label^ _file_name_label;
			TrackBar^ _threshold_trackBar;
			Label^ _path_label;
			Form^ screenshotRegion;
			System::ComponentModel::Container^ components;

			void InitializeComponent();
			void Import_Click(System::Object^ sender, System::EventArgs^ e);
			void ThresholdIntegerLimit(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e);
			void ThresholdValueChanged(System::Object^ sender, System::EventArgs^ e);
			void ThresholdEnterDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e);
			void HandleThresholdChange();
			void SetThresholdTextBox(String^ threshold);
			void ThresholdTrackBarScrolled(System::Object^ sender, System::EventArgs^ e);
			void PanelPaint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e);
			void ScreenShotClick(System::Object^ sender, System::EventArgs^ e);
};
}

#endif