#include "pch.h"
#include "MainForm.h"

namespace CppCLRWinformsProjekt {

	Model model;

	enum ImageFormat {
		JPEG,
		BMP
	};


	MainForm::MainForm() {
		InitializeComponent();
	}

	MainForm::~MainForm()
	{
		if (components)
		{
			delete components;
		}
	}

	void MainForm::Import_Click(System::Object^ sender, System::EventArgs^ e) {
		OpenFileDialog^ file = gcnew OpenFileDialog;
		file->Filter = "csv files (*.csv)|*.csv|All files (*.*)|*.*";
		file->FileName = "";
		file->Title = "Select File";
		file->FilterIndex = 1;

		if (file->ShowDialog() == System::Windows::Forms::DialogResult::OK && file->FileName->Length > 0) {
			_filePathTextBox->Text = file->FileName;
			if (!model.ReadFile(_filePathTextBox->Text)) {
				MessageBox::Show("無法開啟檔案，請再試一次");
			}

			_file_name_label->Text = file->SafeFileName;
			_threshold_value_textBox->Text = "128";
			SetThresholdTextBox("128");
			UpdateView();
		}
	}

	//限制在threshold的textbox中只能輸入數字
	void MainForm::ThresholdIntegerLimit(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e) {
		if (!Char::IsDigit(e->KeyChar) && e->KeyChar != 0x08) {
			e->Handled = true;
		}
	}

	void MainForm::ThresholdValueChanged(System::Object^ sender, System::EventArgs^ e) {
		HandleThresholdChange();
	}

	void MainForm::ThresholdEnterDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
		if (e->KeyCode == Keys::Enter) {
			HandleThresholdChange();
			this->ActiveControl = nullptr;
		}
	}

	void MainForm::HandleThresholdChange() {
		//防止invalid argument
		try {
			SetThresholdTextBox(_threshold_value_textBox->Text);
			UpdateView();
		}
		catch (invalid_argument e) {
			_threshold_value_textBox->Text = "0";
			SetThresholdTextBox(_threshold_value_textBox->Text);
		}
	}

	void MainForm::SetThresholdTextBox(String^ threshold) {
		model.SetThreshold(threshold);
	}

	//當trackBar被移動，透過輸入textbox改變則不會觸發此事件
	void MainForm::ThresholdTrackBarScrolled(System::Object^ sender, System::EventArgs^ e) {
		SetThresholdTextBox(_threshold_trackBar->Value.ToString());
		UpdateView();
	}

	void MainForm::PanelPaint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
		Graphics^ g = e->Graphics;
		vector<vector<int>> vec = model.GetUnboundedPoint();
		for (int i = 0; i < vec.size(); i++)
			g->FillRectangle(Brushes::Black, vec.at(i).at(0), vec.at(i).at(1), 1, 1);

		/*_g = e->Graphics;
		vector<vector<int>> vec = model.GetUnboundedPoint();

		// 分割工作給多個執行緒
		int numThreads = 1; // 使用處理器數量作為執行緒數量
		List<Thread^>^ threads = gcnew List<Thread^>();

		for (int i = 0; i < numThreads; i++) {
			_startIndex = i * (vec.size() / numThreads);
			_endIndex = (i == numThreads - 1) ? vec.size() : (i + 1) * (vec.size() / numThreads);
			Thread^ threadi = gcnew Thread(gcnew ThreadStart(this, &MainForm::Paint));
			threads->Add(threadi);
		}

		// 等待所有執行緒完成
		for each (Thread^ thread in threads) {
			thread->Start();
		}
	}

	void MainForm::Paint() {
		vector<vector<int>> vec = model.GetUnboundedPoint();
		for (int j = _startIndex; j < _endIndex; j++) {
			_g->FillRectangle(Brushes::Black, vec.at(j).at(0), vec.at(j).at(1), 1, 1);
		}
	}*/
	}

	void MainForm::UpdateView() {
		_contact_fraction_value_label->Text = model.GetContactFraction().ToString("f4");
		_unbounded_fraction_value_label->Text = model.GetUnboundedFraction().ToString("f4");
		_threshold_value_textBox->Text = model.GetThreshold().ToString();
		_threshold_trackBar->Value = model.GetThreshold();
		//更新畫布
		_panel->Size = System::Drawing::Size(model.GetLengthOfX(), model.GetLengthOfY());
		_panel->Refresh();
	}

	void MainForm::ScreenShotClick(System::Object^ sender, System::EventArgs^ e) {
		Drawing::Size sz = this->Size;
		Bitmap^ bm = gcnew Bitmap(sz.Width - 20, sz.Height - 10, Imaging::PixelFormat::Format32bppArgb);
		Graphics^ gr = Graphics::FromImage(bm);
		Point src(this->Location.X, this->Location.Y);
		Point dst(-10, -2);
		gr->CopyFromScreen(src, dst, sz);
		delete gr;

		SaveFileDialog^ saveFileDialog = gcnew SaveFileDialog();
		saveFileDialog->FileName = "screenshot.jpg";
		saveFileDialog->Filter = "JPEG Image|*.jpg|Bitmap Image|*.bmp";

		if (saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			switch (saveFileDialog->FilterIndex)
			{
			case ImageFormat::JPEG:
				bm->Save(saveFileDialog->FileName, Imaging::ImageFormat::Jpeg);
				break;
			case ImageFormat::BMP:
				bm->Save(saveFileDialog->FileName, Imaging::ImageFormat::Bmp);
				break;
			default:
				break;
			}
			MessageBox::Show("儲存成功");
		}
		else {
			MessageBox::Show("儲存取消");
		}

		delete bm;
	}


#pragma region Windows Form Designer generated code
	/// <summary>
	/// Erforderliche Methode f�r die Designerunterst�tzung.
	/// Der Inhalt der Methode darf nicht mit dem Code-Editor ge鄚dert werden.
	/// </summary>
	void MainForm::InitializeComponent(void)
	{
		System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
		this->_import_button = (gcnew System::Windows::Forms::Button());
		this->_filePathTextBox = (gcnew System::Windows::Forms::TextBox());
		this->_contact_fraction_label = (gcnew System::Windows::Forms::Label());
		this->_unbounded_fraction_label = (gcnew System::Windows::Forms::Label());
		this->_threshold_label = (gcnew System::Windows::Forms::Label());
		this->_threshold_value_textBox = (gcnew System::Windows::Forms::TextBox());
		this->_contact_fraction_value_label = (gcnew System::Windows::Forms::Label());
		this->_unbounded_fraction_value_label = (gcnew System::Windows::Forms::Label());
		this->_panel = (gcnew System::Windows::Forms::Panel());
		this->_screen_shot_button = (gcnew System::Windows::Forms::Button());
		this->_file_name_title_label = (gcnew System::Windows::Forms::Label());
		this->_file_name_label = (gcnew System::Windows::Forms::Label());
		this->_threshold_trackBar = (gcnew System::Windows::Forms::TrackBar());
		this->_path_label = (gcnew System::Windows::Forms::Label());
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->_threshold_trackBar))->BeginInit();
		this->SuspendLayout();
		// 
		// _import_button
		// 
		this->_import_button->Font = (gcnew System::Drawing::Font(L"Lato", 9.749999F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->_import_button->Location = System::Drawing::Point(648, 46);
		this->_import_button->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
		this->_import_button->Name = L"_import_button";
		this->_import_button->Size = System::Drawing::Size(100, 25);
		this->_import_button->TabIndex = 0;
		this->_import_button->Text = L"Import";
		this->_import_button->UseVisualStyleBackColor = true;
		this->_import_button->Click += gcnew System::EventHandler(this, &MainForm::Import_Click);
		// 
		// _filePathTextBox
		// 
		this->_filePathTextBox->Location = System::Drawing::Point(63, 48);
		this->_filePathTextBox->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
		this->_filePathTextBox->Name = L"_filePathTextBox";
		this->_filePathTextBox->ReadOnly = true;
		this->_filePathTextBox->Size = System::Drawing::Size(579, 22);
		this->_filePathTextBox->TabIndex = 1;
		// 
		// _contact_fraction_label
		// 
		this->_contact_fraction_label->AutoSize = true;
		this->_contact_fraction_label->Font = (gcnew System::Drawing::Font(L"Lato", 15.75F));
		this->_contact_fraction_label->Location = System::Drawing::Point(58, 177);
		this->_contact_fraction_label->Name = L"_contact_fraction_label";
		this->_contact_fraction_label->Size = System::Drawing::Size(167, 25);
		this->_contact_fraction_label->TabIndex = 2;
		this->_contact_fraction_label->Text = L"Contact Fraction";
		// 
		// _unbounded_fraction_label
		// 
		this->_unbounded_fraction_label->AutoSize = true;
		this->_unbounded_fraction_label->Font = (gcnew System::Drawing::Font(L"Lato", 15.75F));
		this->_unbounded_fraction_label->Location = System::Drawing::Point(58, 221);
		this->_unbounded_fraction_label->Name = L"_unbounded_fraction_label";
		this->_unbounded_fraction_label->Size = System::Drawing::Size(203, 25);
		this->_unbounded_fraction_label->TabIndex = 3;
		this->_unbounded_fraction_label->Text = L"Unbounded Fraction";
		// 
		// _threshold_label
		// 
		this->_threshold_label->AutoSize = true;
		this->_threshold_label->Font = (gcnew System::Drawing::Font(L"Lato", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->_threshold_label->Location = System::Drawing::Point(58, 132);
		this->_threshold_label->Name = L"_threshold_label";
		this->_threshold_label->Size = System::Drawing::Size(105, 25);
		this->_threshold_label->TabIndex = 4;
		this->_threshold_label->Text = L"Threshold";
		// 
		// _threshold_value_textBox
		// 
		this->_threshold_value_textBox->Font = (gcnew System::Drawing::Font(L"Lato", 9));
		this->_threshold_value_textBox->ImeMode = System::Windows::Forms::ImeMode::NoControl;
		this->_threshold_value_textBox->Location = System::Drawing::Point(832, 132);
		this->_threshold_value_textBox->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
		this->_threshold_value_textBox->Name = L"_threshold_value_textBox";
		this->_threshold_value_textBox->Size = System::Drawing::Size(86, 22);
		this->_threshold_value_textBox->TabIndex = 5;
		this->_threshold_value_textBox->Text = L"0";
		this->_threshold_value_textBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
		this->_threshold_value_textBox->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MainForm::ThresholdEnterDown);
		this->_threshold_value_textBox->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MainForm::ThresholdIntegerLimit);
		this->_threshold_value_textBox->Validated += gcnew System::EventHandler(this, &MainForm::ThresholdValueChanged);
		// 
		// _contact_fraction_value_label
		// 
		this->_contact_fraction_value_label->Font = (gcnew System::Drawing::Font(L"Lato", 15.75F));
		this->_contact_fraction_value_label->Location = System::Drawing::Point(724, 177);
		this->_contact_fraction_value_label->Name = L"_contact_fraction_value_label";
		this->_contact_fraction_value_label->Size = System::Drawing::Size(201, 32);
		this->_contact_fraction_value_label->TabIndex = 6;
		this->_contact_fraction_value_label->Text = L"Contact Fraction Value";
		this->_contact_fraction_value_label->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
		// 
		// _unbounded_fraction_value_label
		// 
		this->_unbounded_fraction_value_label->Font = (gcnew System::Drawing::Font(L"Lato", 15.75F));
		this->_unbounded_fraction_value_label->Location = System::Drawing::Point(689, 221);
		this->_unbounded_fraction_value_label->Name = L"_unbounded_fraction_value_label";
		this->_unbounded_fraction_value_label->Size = System::Drawing::Size(237, 32);
		this->_unbounded_fraction_value_label->TabIndex = 7;
		this->_unbounded_fraction_value_label->Text = L"Unbounded Fraction Value";
		this->_unbounded_fraction_value_label->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
		// 
		// _panel
		// 
		this->_panel->AutoSize = true;
		this->_panel->BackColor = System::Drawing::Color::White;
		this->_panel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
		this->_panel->Location = System::Drawing::Point(65, 264);
		this->_panel->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
		this->_panel->Name = L"_panel";
		this->_panel->Size = System::Drawing::Size(855, 75);
		this->_panel->TabIndex = 8;
		this->_panel->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MainForm::PanelPaint);
		// 
		// _screen_shot_button
		// 
		this->_screen_shot_button->Font = (gcnew System::Drawing::Font(L"Lato", 15.75F));
		this->_screen_shot_button->Location = System::Drawing::Point(754, 45);
		this->_screen_shot_button->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
		this->_screen_shot_button->Name = L"_screen_shot_button";
		this->_screen_shot_button->Size = System::Drawing::Size(164, 68);
		this->_screen_shot_button->TabIndex = 9;
		this->_screen_shot_button->Text = L"Screenshot";
		this->_screen_shot_button->UseVisualStyleBackColor = true;
		this->_screen_shot_button->Click += gcnew System::EventHandler(this, &MainForm::ScreenShotClick);
		// 
		// _file_name_title_label
		// 
		this->_file_name_title_label->AutoSize = true;
		this->_file_name_title_label->Font = (gcnew System::Drawing::Font(L"Lato", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->_file_name_title_label->Location = System::Drawing::Point(58, 87);
		this->_file_name_title_label->Name = L"_file_name_title_label";
		this->_file_name_title_label->Size = System::Drawing::Size(104, 25);
		this->_file_name_title_label->TabIndex = 10;
		this->_file_name_title_label->Text = L"File Name";
		// 
		// _file_name_label
		// 
		this->_file_name_label->Font = (gcnew System::Drawing::Font(L"Lato", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->_file_name_label->Location = System::Drawing::Point(545, 87);
		this->_file_name_label->Name = L"_file_name_label";
		this->_file_name_label->Size = System::Drawing::Size(201, 32);
		this->_file_name_label->TabIndex = 11;
		this->_file_name_label->Text = L"File Name";
		this->_file_name_label->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
		// 
		// _threshold_trackBar
		// 
		this->_threshold_trackBar->LargeChange = 10;
		this->_threshold_trackBar->Location = System::Drawing::Point(169, 132);
		this->_threshold_trackBar->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
		this->_threshold_trackBar->Maximum = 255;
		this->_threshold_trackBar->Name = L"_threshold_trackBar";
		this->_threshold_trackBar->Size = System::Drawing::Size(657, 45);
		this->_threshold_trackBar->TabIndex = 12;
		this->_threshold_trackBar->TickStyle = System::Windows::Forms::TickStyle::None;
		this->_threshold_trackBar->Scroll += gcnew System::EventHandler(this, &MainForm::ThresholdTrackBarScrolled);
		// 
		// _path_label
		// 
		this->_path_label->AutoSize = true;
		this->_path_label->Font = (gcnew System::Drawing::Font(L"Lato", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->_path_label->Location = System::Drawing::Point(58, 19);
		this->_path_label->Name = L"_path_label";
		this->_path_label->Size = System::Drawing::Size(101, 25);
		this->_path_label->TabIndex = 13;
		this->_path_label->Text = L"Directory";
		// 
		// MainForm
		// 
		this->AutoScaleDimensions = System::Drawing::SizeF(7, 15);
		this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		this->BackColor = System::Drawing::Color::White;
		this->ClientSize = System::Drawing::Size(973, 377);
		this->Controls->Add(this->_path_label);
		this->Controls->Add(this->_threshold_trackBar);
		this->Controls->Add(this->_file_name_label);
		this->Controls->Add(this->_file_name_title_label);
		this->Controls->Add(this->_screen_shot_button);
		this->Controls->Add(this->_panel);
		this->Controls->Add(this->_unbounded_fraction_value_label);
		this->Controls->Add(this->_contact_fraction_value_label);
		this->Controls->Add(this->_threshold_value_textBox);
		this->Controls->Add(this->_threshold_label);
		this->Controls->Add(this->_unbounded_fraction_label);
		this->Controls->Add(this->_contact_fraction_label);
		this->Controls->Add(this->_filePathTextBox);
		this->Controls->Add(this->_import_button);
		this->Font = (gcnew System::Drawing::Font(L"Lato", 8.999999F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
		this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
		this->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
		this->MaximizeBox = false;
		this->MinimizeBox = false;
		this->Name = L"MainForm";
		this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
		this->Text = L"Bounded Fraction Calculator";
		this->TransparencyKey = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(0)),
			static_cast<System::Int32>(static_cast<System::Byte>(64)));
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->_threshold_trackBar))->EndInit();
		this->ResumeLayout(false);
		this->PerformLayout();

	}
#pragma endregion

}
