using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using Emgu.CV;
using Emgu.CV.Structure;
using Emgu.Util;
using ZXing;
using ZXing.Common;
using ZXing.Presentation;
using ZXing.QrCode.Internal;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace SessionManager
{
    public partial class SessionManager : Form
    {         
        private Capture capture;
        private bool captureInProgress;
        private ZXing.BarcodeReader barcodeReader;
        private string lastQrCode;
        private int qrCounter;
        private CancellationTokenSource cancellationTokenSource;
        private SerialPort serialPort;
        private Dictionary<string, int> qrCodeMap;
        private List<byte> serialBuffer = new List<byte>();

        private ConcurrentQueue<string> processPushConfigQueue;
        private ConcurrentQueue<string> processPushControllQueue;
        private ConcurrentQueue<string> processPullQueue;
        private ConcurrentQueue<string> processUISendQueue;
        private ConcurrentQueue<string> processUIRecvQueue;
        private ConcurrentQueue<string> processForward;
        
        private Task HandllerPushTask;
        private Task HandllerUIRecvTask;
        private Task HandllerUISendTask;
        private Task HandllerUIResponseTask;
        private Task HandllerPullTask;
        private Task HandllerForward;

        private PictureBox pictureIndex1;
        private PictureBox pictureIndex2;
        private PictureBox pictureIndex3;
        private PictureBox pictureIndex4;

        //config
        byte[] modehomeConfig    = new byte[] { 0x1F, 0xFF};
        byte[] modesendConfig    = new byte[] { 0x1F, 0xAA};
        byte[] moderecieveConfig = new byte[] { 0x1F, 0xBB};
        byte[] modetestConfig    = new byte[] { 0x1F, 0xCC};
        //controll
        byte[] index1SendControll = new byte[] { 0x2F, 0x01, 0x1F, 0xAA};
        byte[] index2SendControll = new byte[] { 0x2F, 0x02, 0x1F, 0xAA};
        byte[] index3SendControll = new byte[] { 0x2F, 0x03, 0x1F, 0xAA};
        byte[] index4SendControll = new byte[] { 0x2F, 0x04, 0x1F, 0xAA};

        byte[] index1RecvControll = new byte[] { 0x2F, 0x01, 0x1F, 0xBB};
        byte[] index2RecvControll = new byte[] { 0x2F, 0x02, 0x1F, 0xBB};
        byte[] index3RecvControll = new byte[] { 0x2F, 0x03, 0x1F, 0xBB};
        byte[] index4RecvControll = new byte[] { 0x2F, 0x04, 0x1F, 0xBB};
        //response
        byte[] modeResponse = new byte[] {0x3F,0x00,0x00,0x00,0x00,0x00,0x00};
        private struct ClickStatus {
            public bool btnConnect;
            public bool btnDisconnect;
            public bool btnHome;
            public bool btnSend;
            public bool btnRecieve;
        };ClickStatus clickState;

        private struct ConfirmStatus {
            public bool btnSend;
            public bool btnRecieve;
            public bool btnHome;
        };ConfirmStatus confirmState;

        private struct ArrayRepoCheck
        {
            public bool[] arr;
        };ArrayRepoCheck repoCheck;

        private struct QueueConfigStatus
        {
            public string config;
        };QueueConfigStatus queueConfigStatus;

        private struct QueueControllStatus
        {
            public string control;
        };QueueControllStatus queueControllStatus;

        private struct QueueResponseStatus
        {
            public bool[] response;
        }; QueueResponseStatus queueResponseStatus;

        public SessionManager()
        {
            InitializeComponent();
            processPushConfigQueue  = new ConcurrentQueue<string>();
            processPushControllQueue= new ConcurrentQueue<string>();
            processPullQueue        = new ConcurrentQueue<string>();
            processUISendQueue      = new ConcurrentQueue<string>();
            processUIRecvQueue      = new ConcurrentQueue<string>();
            processForward          = new ConcurrentQueue<string>();
            cancellationTokenSource = new CancellationTokenSource();
            serialPort              = new SerialPort();
            qrCodeMap               = new Dictionary<string, int>();
            pictureIndex1           = new PictureBox();
            pictureIndex2           = new PictureBox();
            pictureIndex3           = new PictureBox();
            pictureIndex4           = new PictureBox();
            repoCheck.arr           = new bool[4];
            queueResponseStatus.response = new bool[8];

            try
            {
                capture = new Capture(1);
                barcodeReader = new ZXing.BarcodeReader
                {
                    AutoRotate = true,
                    Options = new DecodingOptions
                    {
                        TryHarder = true,
                        PossibleFormats = new List<BarcodeFormat> { BarcodeFormat.QR_CODE }
                    }
                };
                qrCounter = 1;
                groupCOM.Items.AddRange(new string[] { "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8","COM9", "COM10", "COM14" });
                groupCOM.SelectedIndex = 2;
                groupBaurd.Items.AddRange(new object[] { 9600, 38400, 115200 });
                groupBaurd.SelectedIndex = 2;

                serialPort.DataReceived += SerialPort_DataReceived;

                btnHome.Enabled = false;
                btnGuihang.Enabled = false;
                btnLayhang.Enabled = false;
            }
            catch (Exception ex)
            {
                MessageBox.Show($"errors init camera or barcodeReader: {ex.Message}");
            }
        }

        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                string line = serialPort.ReadLine();
                this.Invoke(new Action(() =>
                {
                    processPullQueue.Enqueue(line);
                }));
            }
            catch (Exception ex)
            {
                Console.WriteLine("Serial receive error: " + ex.Message);
            }
        }

        private async void ProcessForwardHandller(CancellationToken cancellationToken)
        {
            try
            {
                while (!cancellationToken.IsCancellationRequested)
                {
                    if (this == null || cancellationToken.IsCancellationRequested)
                    {
                        break;
                    }
                    if (processForward.TryDequeue(out string qrCode))
                    {
                        Console.WriteLine("Qrcode: " + qrCode);

                        if (!qrCodeMap.ContainsKey(qrCode))
                        {
                            qrCodeMap[qrCode] = qrCounter++;
                        }
                        int mappedValue = qrCodeMap[qrCode];
                        processUISendQueue.Enqueue(mappedValue.ToString());
                    }
                    await Task.Delay(200, cancellationToken);
                }
            }
            catch (TaskCanceledException)
            {
                ;
            }
            catch (Exception ex) {
                Console.WriteLine("Forward Task Exception: " + ex.Message);
            }
        }

        private void ParseSerialResponse(string rawData)
        {
            try
            {
                string[] hexValues = rawData.Trim().Split(' ');
                if (hexValues.Length != 7)
                {
                    return;
                }

                byte[] bytes = new byte[7];
                for (int i = 0; i < 7; i++)
                {
                    bytes[i] = Convert.ToByte(hexValues[i], 16);
                }

                if (bytes[0] != 0x3F)
                {
                    return;
                }

                for (int i = 0; i < 6 && i < queueResponseStatus.response.Length; i++)
                {
                    queueResponseStatus.response[i] = (bytes[i + 1] == 0x01);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("errors parsed : " + ex.Message);
            }
        }

        private async void ProcessPullHandller(CancellationToken cancellationToken)
        {
            try
            {
                while (!cancellationToken.IsCancellationRequested)
                {
                    if (this == null || cancellationToken.IsCancellationRequested)
                    {
                        break;
                    }
                    if (processPullQueue.TryDequeue(out string responsive))
                    {
                        if (serialPort.IsOpen && serialPort != null)
                        {
                            Console.WriteLine("Serial : " + responsive);
                            ParseSerialResponse(responsive);
                        }
                    }
                    await Task.Delay(200);
                }
            }
            catch (TaskCanceledException)
            {
                ;
            }
            catch (Exception ex) {
                Console.WriteLine("Serial Task Pull Exception: " + ex.Message);
            }
        }

        private async void ProcessPushHandller(CancellationToken cancellationToken)
        {
            try
            {
                while (!cancellationToken.IsCancellationRequested)
                {
                    if (this == null || cancellationToken.IsCancellationRequested)
                    {
                        break;
                    }
                    if (processPushConfigQueue.TryDequeue(out string request_config))
                    {
                        if (serialPort.IsOpen && serialPort != null)
                        {
                            if(request_config == "")
                            {
                                ;
                            }
                            if (request_config == "HOME")
                            {
                                serialPort.Write(modehomeConfig, 0, modehomeConfig.Length);
                            }
                            if(request_config == "RECV")
                            {
                                serialPort.Write(moderecieveConfig, 0, moderecieveConfig.Length);
                            }
                            if (request_config == "SEND")
                            {
                                serialPort.Write(modesendConfig, 0, modesendConfig.Length);
                            }
                        }
                    }

                    if(processPushControllQueue.TryDequeue(out string request_controll))
                    {
                        if (serialPort.IsOpen && serialPort != null)
                        {
                            if(request_controll == "")
                            {
                                ;
                            }
                            if(request_controll == "SEND1")
                            {
                                serialPort.Write(index1SendControll, 0, index1SendControll.Length);
                            }
                            if (request_controll == "SEND2")
                            {
                                serialPort.Write(index2SendControll, 0, index2SendControll.Length);
                            }
                            if (request_controll == "SEND3")
                            {
                                serialPort.Write(index3SendControll, 0, index3SendControll.Length);
                            }
                            if (request_controll == "SEND4")
                            {
                                serialPort.Write(index4SendControll, 0, index4SendControll.Length);
                            }
                            if (request_controll == "RECV1")
                            {
                                serialPort.Write(index1RecvControll, 0, index1RecvControll.Length);
                            }
                            if (request_controll == "RECV2")
                            {
                                serialPort.Write(index2RecvControll, 0, index2RecvControll.Length);
                            }
                            if (request_controll == "RECV3")
                            {
                                serialPort.Write(index3RecvControll, 0, index3RecvControll.Length);
                            }
                            if (request_controll == "RECV4")
                            {
                                serialPort.Write(index4RecvControll, 0, index4RecvControll.Length);
                            }
                        }
                    }
                    await Task.Delay(50, cancellationToken);
                }
            }
            catch (TaskCanceledException)
            {
                ;
            }
            catch (Exception ex)
            {
                Console.WriteLine("Serial Task Push Exception: " + ex.Message);
            }
        }



        private void SetLedStateMode(Color connect, Color guiHangColor, Color layHangColor, Color homeColor)
        {
            ledConnect.BackColor = connect;
            ledGuihang.BackColor = guiHangColor;
            ledLayhang.BackColor = layHangColor;
            ledHome.BackColor = homeColor;
        }

        private void SetLedStateResponse(Color btnX, Color btnY, Color btnZ, Color motoX, Color motoY, Color motoZ)
        {
            ledBtnX.BackColor = btnX;
            ledBtnY.BackColor = btnY;
            ledBtnZ.BackColor = btnZ;
            ledMotoX.BackColor = motoX;
            ledMotoY.BackColor = motoY;
            ledMotoZ.BackColor = motoZ;
        }

        void HandleReceiveMode()
        {
        //lấy hàng .....
            for (int i = 0; i < repoCheck.arr.Length; i++)
            {
                if (repoCheck.arr[i] == true)
                {
                    Panel targetPanel = null;
                    switch (i)
                    {
                        case 0: targetPanel = index1; break;
                        case 1: targetPanel = index2; break;
                        case 2: targetPanel = index3; break;
                        case 3: targetPanel = index4; break;
                        default:continue;
                    }

                    bool alreadyHasButton = targetPanel.Controls
                        .OfType<System.Windows.Forms.Button>()
                        .Any(btn => btn.Tag?.ToString() == "btnGet");

                    if (alreadyHasButton)
                        continue;

                    System.Windows.Forms.Button btnGet = new System.Windows.Forms.Button();
                    btnGet.Text = "Lấy hàng";
                    btnGet.Size = new Size(80, 30);
                    btnGet.Tag = "btnGet";

                    btnGet.Location = new Point(
                        targetPanel.Width - btnGet.Width - 5,
                        targetPanel.Height - btnGet.Height - 0
                    );

                    int capturedIndex = i;
                    btnGet.Click += (s, e) =>
                    {
                        queueControllStatus.control = "RECV" + (capturedIndex+1);
                        processPushControllQueue.Enqueue(queueControllStatus.control);
                        repoCheck.arr[capturedIndex] = false;
                        targetPanel.Controls.Remove((Control)s);
                        var picture = targetPanel.Controls.OfType<PictureBox>().FirstOrDefault();
                        if (picture != null)
                        {
                            targetPanel.Controls.Remove(picture);
                        }
                    };
                    targetPanel.Controls.Add(btnGet);
                }
            }
        }

        void HandleSendMode(String indexmap,String qr)
        {   
            //gửi hàng ..... 
            if (indexmap == "1" && confirmState.btnSend && !confirmState.btnRecieve)
            {
                repoCheck.arr[0] = true;
                pictureIndex1.Location = new Point(
                    (index1.Width - pictureIndex1.Width) / 2,
                    (index1.Height - pictureIndex1.Height) / 2
                );
                index1.Controls.Clear();
                index1.Controls.Add(pictureIndex1);
                queueControllStatus.control = "SEND1";
                processPushControllQueue.Enqueue(queueControllStatus.control);
            }

            if (indexmap == "2" && confirmState.btnSend && !confirmState.btnRecieve)
            {
                repoCheck.arr[1] = true;
                pictureIndex2.Location = new Point(
                    (index2.Width - pictureIndex2.Width) / 2,
                    (index2.Height - pictureIndex2.Height) / 2
                );
                index2.Controls.Clear();
                index2.Controls.Add(pictureIndex2);
                queueControllStatus.control = "SEND2";
                processPushControllQueue.Enqueue(queueControllStatus.control);
            }

            if (indexmap == "3" && confirmState.btnSend && !confirmState.btnRecieve)
            {
                repoCheck.arr[2] = true;
                pictureIndex3.Location = new Point(
                    (index3.Width - pictureIndex3.Width) / 2,
                    (index3.Height - pictureIndex3.Height) / 2
                );
                index3.Controls.Clear();
                index3.Controls.Add(pictureIndex3);
                queueControllStatus.control = "SEND3";
                processPushControllQueue.Enqueue(queueControllStatus.control);
            }

            if (indexmap == "4" && confirmState.btnSend && !confirmState.btnRecieve)
            {
                repoCheck.arr[3] = true;
                pictureIndex4.Location = new Point(
                    (index4.Width - pictureIndex4.Width) / 2,
                    (index4.Height - pictureIndex4.Height) / 2
                );          
                index4.Controls.Clear();
                index4.Controls.Add(pictureIndex4);
                queueControllStatus.control = "SEND4";
                processPushControllQueue.Enqueue(queueControllStatus.control);
            }
        }
        private async void ProcessUISend(CancellationToken cancellationToken)
        {
            try
            {
                while (!cancellationToken.IsCancellationRequested)
                {
                    if (this == null || cancellationToken.IsCancellationRequested)
                    {
                        break;
                    }
                    if (processUISendQueue.TryDequeue(out string indexmap))
                    {
                        this.Invoke((MethodInvoker)delegate
                        {
                            HandleSendMode(indexmap, lastQrCode);
                        });
                    }
                    await Task.Delay(100, cancellationToken);
                }
            }
            catch (TaskCanceledException)
            {
                ;
            }
            catch (Exception ex)
            {
                Console.WriteLine("UI Send Task Exception: " + ex.Message);
            }
        }

        private async void ProcessUIResponse(CancellationToken cancellationToken)
        {
            try
            {
                while (!cancellationToken.IsCancellationRequested)
                {
                    if (this == null || cancellationToken.IsCancellationRequested)
                    {
                        break;
                    }
                    this.Invoke((MethodInvoker)delegate
                    {
                        if (queueResponseStatus.response != null && queueResponseStatus.response.Length >= 6)
                        {
                            Color btnX = queueResponseStatus.response[0] ? Color.Green : Color.Red;
                            Color btnY = queueResponseStatus.response[1] ? Color.Green : Color.Red;
                            Color btnZ = queueResponseStatus.response[2] ? Color.Green : Color.Red;
                            Color motoX = queueResponseStatus.response[3] ? Color.Green : Color.Red;
                            Color motoY = queueResponseStatus.response[4] ? Color.Green : Color.Red;
                            Color motoZ = queueResponseStatus.response[5] ? Color.Green : Color.Red;
                            SetLedStateResponse(btnX, btnY, btnZ, motoX, motoY, motoZ);
                        }
                    });
                    await Task.Delay(100, cancellationToken);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("UI Response Task Exception: " + ex.Message);
            }
        }


        private async void ProcessUIRecv(CancellationToken cancellationToken)
        {
            try
            {
                while (!cancellationToken.IsCancellationRequested)
                {
                    if (this == null || cancellationToken.IsCancellationRequested)
                    {
                        break;
                    }
                    this.Invoke((MethodInvoker)delegate
                    {
                        pictureIndex1.Image = CameraCapture.Properties.Resources.boxqrcode;
                        pictureIndex1.SizeMode = PictureBoxSizeMode.Zoom;
                        pictureIndex1.Size = new Size(150, 150);

                        pictureIndex2.Image = CameraCapture.Properties.Resources.boxqrcode;
                        pictureIndex2.SizeMode = PictureBoxSizeMode.Zoom;
                        pictureIndex2.Size = new Size(150, 150);

                        pictureIndex3.Image = CameraCapture.Properties.Resources.boxqrcode;
                        pictureIndex3.SizeMode = PictureBoxSizeMode.Zoom;
                        pictureIndex3.Size = new Size(150, 150);

                        pictureIndex4.Image = CameraCapture.Properties.Resources.boxqrcode;
                        pictureIndex4.SizeMode = PictureBoxSizeMode.Zoom;
                        pictureIndex4.Size = new Size(150, 150);
                 
                        if (clickState.btnConnect)
                        {
                            clickState.btnConnect = false;
                            btnHome.Enabled = true;
                            btnGuihang.Enabled = true;
                            btnLayhang.Enabled = true;
                            SetLedStateMode(Color.Green, Color.Red, Color.Red, Color.Red);
                        }
                        else if (clickState.btnDisconnect)
                        {
                            clickState.btnDisconnect = false;
                            btnHome.Enabled = false;
                            btnGuihang.Enabled = false;
                            btnLayhang.Enabled = false;
                            SetLedStateMode(Color.Red,Color.Red, Color.Red, Color.Red);
                            SetLedStateResponse(Color.Red, Color.Red, Color.Red, Color.Red, Color.Red, Color.Red);
                        }

                        if (clickState.btnSend)
                        {
                            SetLedStateMode(Color.Green,Color.Green, Color.Red, Color.Red);
                            DialogResult result = MessageBox.Show(
                                "Vui lòng :\r\nĐặt hàng hóa chứa mã QR hợp lệ trước camera." +
                                "          \r\nCăn chỉnh ánh sáng hợp lý để camera hoạt động tốt nhất\r\n" +
                                "          \r\nNhấn “Yes” để bắt đầu quy trình",
                                "Bạn đang chọn chế độ “Gửi hàng”",                        
                                MessageBoxButtons.YesNo,
                                MessageBoxIcon.Question
                            );
                            confirmState.btnSend = (result == DialogResult.Yes);
                            confirmState.btnHome    = false;
                            confirmState.btnRecieve = false;
                            clickState.btnSend = false;
                            if (confirmState.btnSend)
                            {
                                queueConfigStatus.config = "SEND";
                                processPushConfigQueue.Enqueue(queueConfigStatus.config);
                            }         
                        }
                        else if (clickState.btnRecieve)
                        {
                            SetLedStateMode(Color.Green,Color.Red, Color.Green, Color.Red);
                            DialogResult result = MessageBox.Show(
                                "Vui lòng :\r\nChọn hàng khu vực vị trí hàng muốn lấy" +
                                "          \r\nSau đó nhấn nút “lấy hàng” \r\n" +
                                "           \r\nNhấn “Yes” để bắt đầu quy trình",
                                "Bạn đang chọn chế độ “Lấy hàng”",
                                MessageBoxButtons.YesNo,
                                MessageBoxIcon.Question
                            );
                            confirmState.btnRecieve = (result == DialogResult.Yes);
                            confirmState.btnHome = false;
                            confirmState.btnSend = false;
                            clickState.btnRecieve = false;
                            if (confirmState.btnRecieve)
                            {
                                queueConfigStatus.config = "RECV";
                                processPushConfigQueue.Enqueue(queueConfigStatus.config);
                                HandleReceiveMode();
                            }
                        }
                        else if (clickState.btnHome)
                        {
                            SetLedStateMode(Color.Green,Color.Red, Color.Red, Color.Green);
                            confirmState.btnHome    = true;
                            confirmState.btnRecieve = false;
                            confirmState.btnSend    = false;
                            clickState.btnHome      = false;
                            if (confirmState.btnHome)
                            {
                                queueConfigStatus.config = "HOME";
                                processPushConfigQueue.Enqueue(queueConfigStatus.config);
                            }
                        }
                    });
                    await Task.Delay(100, cancellationToken);
                }
            }
            catch (TaskCanceledException)
            {
                ;
            }
            catch (Exception ex)
            {
                Console.WriteLine("UI Recv Task Exception: " + ex.Message);
            }
        }


        private void ProcessFrame(object sender, EventArgs e)
        {
            try
            {
                if (capture == null)
                {
                    return;
                }

                Image<Bgr, Byte> ImageFrame = capture.QueryFrame();
                if (ImageFrame == null || ImageFrame.Width == 0 || ImageFrame.Height == 0)
                {
                    return;
                }

                CamImageBox.Image = ImageFrame;

                using (Bitmap bitmap = ImageFrame.ToBitmap())
                {
                    if (bitmap == null)
                    {
                        return;
                    }

                    if (barcodeReader == null)
                    {
                        return;
                    }

                    var result = barcodeReader.Decode(bitmap);
                    if (result != null && result.Text != lastQrCode)
                    {
                        lastQrCode = result.Text;
                        processForward.Enqueue(lastQrCode);
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"errors handler frames: {ex.Message}");
            }
        }


        private void btnStart_Click(object sender, EventArgs e)
        {
            if (capture == null)
                return;

            if (!serialPort.IsOpen && !captureInProgress)
            {
                try
                {
                    serialPort.PortName = groupCOM.Text;
                    serialPort.BaudRate = Convert.ToInt32(groupBaurd.Text);
                    serialPort.Open();

                    btnConnect.Text = "Ngắt kết nối";
                    Application.Idle += ProcessFrame;
                    captureInProgress = true;

                    clickState.btnConnect = true;
                    clickState.btnDisconnect = false;
                }
                catch (Exception ex)
                {
                    Console.WriteLine("errors open COM: " + ex.Message);
                }
            }
            else
            {
                try
                {
                    Application.Idle -= ProcessFrame;
                    captureInProgress = false;

                    if (serialPort.IsOpen)
                        serialPort.Close();

                    serialPort.Dispose();
                    serialPort = new SerialPort();
                    serialPort.DataReceived += SerialPort_DataReceived;

                    btnConnect.Text = "Kết nối";

                    clickState.btnConnect = false;
                    clickState.btnDisconnect = true;
                }
                catch (Exception ex)
                {
                    Console.WriteLine("errors closed COM: " + ex.Message);
                }
            }
        }

        private void CameraCapture_Load(object sender, EventArgs e)
        {
            HandllerPushTask = Task.Run(()       => ProcessPushHandller(cancellationTokenSource.Token));
            HandllerUISendTask = Task.Run(()     => ProcessUISend(cancellationTokenSource.Token));
            HandllerUIResponseTask = Task.Run(() => ProcessUIResponse(cancellationTokenSource.Token));
            HandllerUIRecvTask = Task.Run(()     => ProcessUIRecv(cancellationTokenSource.Token));
            HandllerPullTask = Task.Run(()       => ProcessPullHandller(cancellationTokenSource.Token));
            HandllerForward = Task.Run(()        => ProcessForwardHandller(cancellationTokenSource.Token));
        }

        private void CamImageBox_Click(object sender, EventArgs e)
        {
            ;
        }

        private void btnGuihang_Click(object sender, EventArgs e)
        {
            clickState.btnSend = true;
        }

        private void btnLayhang_Click(object sender, EventArgs e)
        {
            clickState.btnRecieve = true;
        }

        private void btnHome_Click(object sender, EventArgs e)
        {
            clickState.btnHome = true;
        }

        private void label7_Click(object sender, EventArgs e)
        {
            ;
        }

        private void label9_Click(object sender, EventArgs e)
        {
            ;
        }

        private void ReleaseData()
        {
            if (serialPort != null && serialPort.IsOpen)
            {
                serialPort.Close();
                serialPort.Dispose();
            }

            if (capture != null)
            {
                capture.Dispose();
            }
        }


        private async void SessionManager_FormClosing(object sender, FormClosingEventArgs e)
        {
            var result = MessageBox.Show("Bạn có chắc chắn muốn đóng ứng dụng?", "Xác nhận", MessageBoxButtons.YesNo, MessageBoxIcon.Question);

            if (result == DialogResult.No)
            {
                e.Cancel = true;
                return;
            }

            cancellationTokenSource?.Cancel();

            try
            {
                await Task.WhenAll(
                    HandllerPushTask ?? Task.CompletedTask,
                    HandllerUIRecvTask ?? Task.CompletedTask,
                    HandllerUISendTask ?? Task.CompletedTask,
                    HandllerUIResponseTask ?? Task.CompletedTask,
                    HandllerPullTask ?? Task.CompletedTask,
                    HandllerForward ?? Task.CompletedTask
                );
            }
            catch (TaskCanceledException)
            {
                Console.WriteLine("All task exception: A task was canceled.");
            }
            catch (ObjectDisposedException ex)
            {
                Console.WriteLine($"Task Exception: {ex.Message}");
            }
            finally
            {
                ReleaseData();
                Console.WriteLine("Resources released successfully.");
            }
            e.Cancel = false;
        }

    }
}