using Avalonia;
using Avalonia.Controls;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Interactivity;

namespace AssetTool.Views;

public partial class MainView : UserControl {
    public MainView() {
        InitializeComponent();
    }

    private void BtnExportPak_OnClick(object? sender, RoutedEventArgs e) {
        return;
    }

    private void MenuBtnExit_OnClick(object? sender, RoutedEventArgs e) {
        if (Application.Current?.ApplicationLifetime is IClassicDesktopStyleApplicationLifetime lifetime) {
            lifetime.Shutdown(0);
        }
    }
}