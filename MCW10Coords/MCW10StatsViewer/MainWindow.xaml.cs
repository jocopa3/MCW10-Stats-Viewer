using System;
using System.Windows;
using System.Windows.Threading;
using System.Runtime.InteropServices;
using System.Reflection;
using System.IO;
using System.Threading.Tasks;
using System.Collections.Generic;
using System.Collections.ObjectModel;

using MahApps.Metro.Controls;
using MahApps.Metro.Controls.Dialogs;
using System.Windows.Controls;
using System.Windows.Input;
using System.Text;
using System.ComponentModel;

namespace MCW10StatsViewer
{
    public static class ExtensionMethods
    {
        private static Action Delegate = delegate () { };

        public static void Refresh(this UIElement element)
        {
            element.Dispatcher.Invoke(DispatcherPriority.Render, Delegate);
        }
    }

    public partial class MainWindow : MetroWindow
    {
        private StateChangeCallback test;

        private DispatcherTimer dispatcherTimer;
        public int refreshTime = 1000 / 60; // Update 60 times a second

        private ObservableCollection<TreeItem> Items { get; set; }

        public MainWindow()
        {
            InitializeComponent();
            LauncherWrapper.Initialize();
            test = new StateChangeCallback(this);

            Items = new ObservableCollection<TreeItem>();
            InitItems();
            StatsView.ItemsSource = Items;

            dispatcherTimer = new DispatcherTimer();
            dispatcherTimer.Tick += new EventHandler(dispatcherTimer_Tick);
            dispatcherTimer.Interval = new TimeSpan(0, 0, 0, 0, refreshTime);
            dispatcherTimer.Start();

            Closing += Close_Window;
        }

        public void InitItems()
        {
            Items.Clear();

            CompoundItem renderStats = new CompoundItem() { Name = "Rendering Stats" };
            {
                renderStats.Items.Add(new PrimitiveItem<float>() { Name = "FPS", GetValue = LauncherWrapper.GetFPS, Description = "Average frames per second" });
                renderStats.Items.Add(new PrimitiveItem<int>() { Name = "Visible Entities", GetValue = LauncherWrapper.GetEntitiesOnScreen, Description = "Number of entities being rendered on the screen" });
            }
            Items.Add(renderStats);

            CompoundItem playerStats = new CompoundItem() { Name = "Player Stats", Description = "Stats about the local player" };
            {
                CompoundValueItem<Vec3> playerPositionStats = new CompoundValueItem<Vec3>() { Name = "Position", GetValue = LauncherWrapper.GetPlayerPosition, Description = "Position of the player using world coordinates", IsExpanded = false };
                {
                    playerPositionStats.Items.Add(new PrimitiveItem<Vec3>() { Name = "Block", GetValue = LauncherWrapper.GetPlayerBlockPosition, Description = "Position of the player to the nearest block" });
                    playerPositionStats.Items.Add(new PrimitiveItem<Vec3>() { Name = "Chunk", GetValue = LauncherWrapper.GetPlayerChunkPosition, Description = "Position of the player to the nearest chunk" });
                }
                playerStats.Items.Add(playerPositionStats);

                CompoundValueItem<Vec2> rotationStats = new CompoundValueItem<Vec2>() { Name = "Rotation", GetValue = LauncherWrapper.GetPlayerRotation, Description = "The rotation of the player", IsExpanded = false };
                {
                    rotationStats.Items.Add(new PrimitiveItem<float>() { Name = "Pitch", GetValue = LauncherWrapper.GetPlayerPitch, Description = "Up/down rotation" });
                    rotationStats.Items.Add(new PrimitiveItem<float>() { Name = "Yaw", GetValue = LauncherWrapper.GetPlayerYaw, Description = "Y-Axis rotation"});
                    rotationStats.Items.Add(new EnumItem<BlockFace>() { Name = "Facing", GetValue = LauncherWrapper.GetPlayerFacingDirection, Description = "Direction the player is facing" });
                }
                playerStats.Items.Add(rotationStats);

                playerStats.Items.Add(new PrimitiveItem<int>() { Name = "Age", GetValue = LauncherWrapper.GetPlayerAge, Description = "How old the player is (measured in game ticks)" });
                playerStats.Items.Add(new PrimitiveItem<float>() { Name = "Distance Travelled", GetValue = LauncherWrapper.GetPlayerDistanceTravelled, Description = "Distance the player travelled on foot while playing the level" });
                playerStats.Items.Add(new StructItem<IntVec2>() { Name = "Sky/Block Light", GetValue = LauncherWrapper.GetLightValues, Description = "Sky and block light values for the block the player is looking at (Not always accurate)" });

                CompoundItem itemInHandStats = new CompoundEnumItem<DataValue>() { Name = "Item In Hand", GetValue = LauncherWrapper.GetItemIdEnum, Description = "Information about the item the player is holding", IsExpanded = false };
                {
                    itemInHandStats.Items.Add(new PrimitiveItem<short>() { Name = "Id", GetValue = LauncherWrapper.GetItemId, Description = "The Id of the item" });
                    itemInHandStats.Items.Add(new PrimitiveItem<short>() { Name = "Damage", GetValue = LauncherWrapper.GetItemDamage, Description = "Extra data or damage used to define the item" });
                    itemInHandStats.Items.Add(new PrimitiveItem<short>() { Name = "Count", GetValue = LauncherWrapper.GetItemAmount, Description = "How much of the item the player has" });
                }
                playerStats.Items.Add(itemInHandStats);

                CompoundValueItem<HitResult> lookingAtInfo = new CompoundValueItem<HitResult>() { Name = "Looking At", GetValue = LauncherWrapper.GetHitResultType, Description = "The object the player is currently looking at", IsExpanded = false };
                {
                    lookingAtInfo.Items.Add(new StructItem<Vec3>() { Name = "Position", GetValue = LauncherWrapper.GetPlayerLookingAt, Description = "Position of where the player is looking" });
                    lookingAtInfo.Items.Add(new HitResultItem() { Name = "Info", GetValue = LauncherWrapper.GetHitResultType, Description = "Extra data about the object the player is looking at" });
                }
                playerStats.Items.Add(lookingAtInfo);

                int totalAttributes = LauncherWrapper.GetPlayerAttributesLength();
                if (totalAttributes > 0 && totalAttributes != 31)
                {
                    CompoundItem attributeStats = new CompoundItem() { Name = "Attributes", Description = "Player attributes", IsExpanded = false };
                    {
                        for (int ind = 0; ind < totalAttributes; ind++)
                        {
                            StringBuilder attributeName = new StringBuilder(31);

                            int length = LauncherWrapper.GetPlayerAttributeName(ind, attributeName);
                            string name = attributeName.ToString();
                            if (String.IsNullOrEmpty(name))
                                continue;

                            int strInd = name.IndexOf(".") + 1;
                            name = name.Substring(strInd, length - strInd);
                            name = char.ToUpper(name[0]) + name.Substring(1);

                            attributeStats.Items.Add(new AttributeItem() { Name = name, Index = ind, GetValue = LauncherWrapper.GetPlayerAttributValues, Description = EntityUtils.getAttributeDescription(name) });
                        }
                    }
                    playerStats.Items.Add(attributeStats);
                }

            }
            Items.Add(playerStats);

            
            CompoundItem levelStats = new CompoundItem() { Name = "Level Stats", Description = "Stats about the current level" };
            {
                levelStats.Items.Add(new StringItem() { Name = "Level Name", GetValue = LauncherWrapper.GetLevelName, GetLength = LauncherWrapper.GetLevelNameLength, MaxLength = 40, Description = "The name of the current level" });
                levelStats.Items.Add(new TimeItem() { Name = "Time", GetValueA = LauncherWrapper.GetLevelTime, GetValueB = LauncherWrapper.GetLevelTimeOfDay, Description = "Number of ticks since the world was started; used in-game to keep track of time" });
                /*
                CompoundItem weatherStats = new CompoundItem() { Name = "Weather", Description = "Stats about the weather" };
                {
                    
                }
                */
            }
            Items.Add(levelStats);
        }

        private void TreeViewPreviewMouseMove(object sender, System.Windows.Input.MouseEventArgs e)
        {
            TreeViewItem control = GetHitControl((TreeView)sender, null);
            if (control != null)
            {
                InfoLabel.Text = "Info: "+((TreeItem)(control.Header)).Description;
            }
        }

        private static TreeViewItem GetHitControl(TreeView listControl, MouseButtonEventArgs e)
        {
            Point hit;

            if (e == null)
                hit = Mouse.GetPosition(listControl);
            else
                hit = e.GetPosition(listControl);
            object obj = listControl.InputHitTest(hit);

            if ((obj != null) && (obj is FrameworkElement))
            {
                object control = obj;
                while (control != null)
                {
                    if (control.GetType().GetProperty("TemplatedParent").GetValue(control, null) != null)
                        control = (FrameworkElement)obj.GetType().GetProperty("TemplatedParent").GetValue(control, null);
                    else if (control == listControl)
                        break;
                    else if (control is FrameworkElement)
                        control = ((FrameworkElement)control).Parent;
                    else
                        break;

                    if (control is TreeViewItem)
                    {
                        return control as TreeViewItem;
                    }
                }
            }
            return null;
        }

        private void dispatcherTimer_Tick(object sender, EventArgs e)
        {
            updateStats();
        }

        private void updateCoords(object sender, RoutedEventArgs e)
        {
            updateStats();
        }

        private int attributeLength = 0;

        private void updateStats()
        {
            bool inLevel = LauncherWrapper.IsInLevel();

            if(!inLevel)
            {
                // Update FPS only
                ((CompoundItem)Items[0]).Items[0].Update();
                return;
            } else
            {
                // Update all stats
                int attributes = LauncherWrapper.GetPlayerAttributesLength();

                if (attributes != attributeLength)
                {
                    InitItems();
                }

                attributeLength = attributes;

                LauncherWrapper.updatePlayer();

                foreach (TreeItem item in Items)
                {
                    item.Update();
                }
            }
        }

        AboutWindow window;

        public void ShowAboutWindow(object sender, RoutedEventArgs e)
        {
            window = new AboutWindow();
            window.Show();
        }

        public void Close_Window(object sender, CancelEventArgs e)
        {
            if (window != null)
                window.Close();
        }
    }
}
