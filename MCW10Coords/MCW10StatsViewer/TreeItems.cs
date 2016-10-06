using System;
using System.Collections.Generic;
using System.Windows;
using System.IO;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows.Media;
using System.Text;
using System.Globalization;

namespace MCW10StatsViewer
{

    public class TreeItem : INotifyPropertyChanged
    {
        public string Name { get; set; }
        public string Value { get; set; }
        public string Description { get; set; }

        public event PropertyChangedEventHandler PropertyChanged;

        public void NotifyPropertyChanged(String info)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(info));
            }
        }

        public virtual void Update() { }

        public delegate void ValueGetter();
        public ValueGetter GetValue { get; set; }
    }

    public class PrimitiveItem<T> : TreeItem
    {
        public override void Update()
        {
            Value = GetValue();
            NotifyPropertyChanged("Value");
        }

        new public T Value { get; set; }

        new public delegate T ValueGetter();
        new public ValueGetter GetValue { get; set; }
    }

    public class StructItem<T> : TreeItem where T : new()
    {
        public override void Update()
        {
            T t = new T();
            GetValue(ref t);

            Value = t;
            NotifyPropertyChanged("Value");
        }

        new public T Value { get; set; }

        new public delegate void ValueGetter(ref T t);
        new public ValueGetter GetValue { get; set; }
    }

    public class AttributeItem : TreeItem
    {
        public override void Update()
        {
            Vec2 vec2 = new Vec2();
            GetValue(Index, ref vec2);

            Value = vec2;
            NotifyPropertyChanged("Value");
        }

        new public Vec2 Value { get; set; }
        public int Index { get; set; }

        new public delegate void ValueGetter(int index, ref Vec2 vec2);
        new public ValueGetter GetValue { get; set; }
    }

    public class StringItem : TreeItem
    {
        public override void Update()
        {
            int len = GetLength();

            // Prevent some nasty issues
            if (len <= 0 || len > MaxLength)
                return;

            var strBuffer = new byte[len];
            GetValue(ref strBuffer[0], len);

            Value = Encoding.UTF8.GetString(strBuffer);
            len = Value.Length;

            Value = Value.Substring(0, len);
            NotifyPropertyChanged("Value");
        }

        new public string Value { get; set; }
        public int MaxLength { get; set; }

        new public delegate void ValueGetter(ref byte str, int maxLen);
        new public ValueGetter GetValue { get; set; }

        public delegate int LengthGetter();
        public LengthGetter GetLength { get; set; }
    }

    public class ColorItem : TreeItem
    {
        public override void Update()
        {
            Value = GetValue();
            NotifyPropertyChanged("Value");

            color = Color.FromRgb((byte)Value.x, (byte)Value.y, (byte)Value.z);
            NotifyPropertyChanged("color");
        }

        new public IntVec3 Value { get; set; }
        public Color color { get; set; }

        new public delegate IntVec3 ValueGetter();
        new public ValueGetter GetValue { get; set; }
    }

    public class EnumItem<T> : TreeItem
    {
        public override void Update()
        {
            Value = GetValue().ToString().Replace("_", " ");
            NotifyPropertyChanged("Value");
        }

        new public string Value { get; set; }

        new public delegate T ValueGetter();
        new public ValueGetter GetValue { get; set; }
    }

    public class TimeItem : TreeItem
    {
        public override void Update()
        {
            Value = GetValueA();
            NotifyPropertyChanged("Value");

            this.Time = LevelUtils.TimeAsEnum(GetValueB());
            NotifyPropertyChanged("Time");
        }

        new public long Value { get; set; }
        public TimeOfDay Time { get; set; }

        public delegate long ValueGetterA();
        public ValueGetterA GetValueA { get; set; }

        public delegate long ValueGetterB();
        public ValueGetterB GetValueB { get; set; }
    }

    public class CompoundItem: TreeItem
    {
        public bool isexpanded = true;
        public bool IsExpanded
        {
            get { return isexpanded; }
            set
            {
                isexpanded = value;
                NotifyPropertyChanged("IsExpanded");
                NotifyPropertyChanged("Isexpanded");
            }
        }

        public CompoundItem()
        {
            Items = new ObservableCollection<TreeItem>();
        }

        public override void Update()
        {
            if (!this.isexpanded)
                return;

            foreach (TreeItem item in Items)
            {
                item.Update();
            }
            NotifyPropertyChanged("Items");
        }

        public ObservableCollection<TreeItem> Items { get; set; }
    }

    public class CompoundValueItem<T> : CompoundItem
    {
        public override void Update()
        {
            if (this.isexpanded)
            {
                foreach (TreeItem item in Items)
                {
                    item.Update();
                }
                NotifyPropertyChanged("Items");
            }

            Value = GetValue();
            NotifyPropertyChanged("Value");
        }
        
        new public T Value { get; set; }

        new public delegate T ValueGetter();
        new public ValueGetter GetValue { get; set; }
    }

    public class CompoundObjectItem<T> : CompoundItem where T : new()
    {
        public override void Update()
        {
            if (this.isexpanded)
            {
                foreach (TreeItem item in Items)
                {
                    item.Update();
                }
                NotifyPropertyChanged("Items");
            }

            T val = new T();
            GetValue(ref val);

            Value = val;
            NotifyPropertyChanged("Value");
        }

        new public T Value { get; set; }

        new public delegate void ValueGetter(ref T value);
        new public ValueGetter GetValue { get; set; }
    }

    public class CompoundEnumItem<T> : CompoundItem
    {
        public override void Update()
        {
            if (this.isexpanded)
            {

                foreach (TreeItem item in Items)
                {
                    item.Update();
                }
                NotifyPropertyChanged("Items");
            }

            Value = GetValue().ToString().Replace("_", " ");
            Value = CultureInfo.CurrentCulture.TextInfo.ToTitleCase(Value.ToLower());
            NotifyPropertyChanged("Value");
        }

        new public delegate T ValueGetter();
        new public ValueGetter GetValue { get; set; }
    }

    public class HitResultItem : CompoundValueItem<HitResult>
    {
        public HitResultItem()
        {
            //blockStats = new ObservableCollection<TreeItem>();
            //entityStats = new ObservableCollection<TreeItem>();

            /*
            blockStats.Add(new IntVec3Item() { Name = "Block Position", GetValue = LauncherWrapper.GetHitBlockPosition });
            blockStats.Add(new EnumItem<BlockFace>() { Name = "Block Face", GetValue = LauncherWrapper.GetHitFace });

            entityStats.Add(new EnumItem<EntityRenderModel>() { Name = "Type", GetValue = LauncherWrapper.GetEntityRenderModel });
            entityStats.Add(new Vec3AItem() { Name = "Position", GetValue = LauncherWrapper.GetEntityPosition });
            entityStats.Add(new Vec3AItem() { Name = "Velocity", GetValue = LauncherWrapper.GetEntityVelocity });
            entityStats.Add(new Vec2AItem() { Name = "Rotation", GetValue = LauncherWrapper.GetEntityRotation });
            entityStats.Add(new ColorItem() { Name = "Color", GetValue = LauncherWrapper.GetEntityColor });
            */
        }

        //private ObservableCollection<TreeItem> blockStats;
        //private ObservableCollection<TreeItem> entityStats;

        bool expanded = false;

        public override void Update()
        {
            HitResult newValue = GetValue();

            if (isexpanded)
            {
                if (LauncherWrapper.IsInLevel())
                    LauncherWrapper.updateEntity();

                if (newValue != Value || expanded != isexpanded)
                {
                    updateHitStats(newValue);

                    NotifyPropertyChanged("Items");

                    expanded = isexpanded;
                }

                foreach (TreeItem item in Items)
                {
                    item.Update();
                }

            }

            Value = newValue;
            NotifyPropertyChanged("Value");
        }

        private void updateHitStats(HitResult newValue)
        {
            Items.Clear();
            switch (newValue)
            {
                case HitResult.Block:
                    //Items = blockStats;
                    Items.Add(new StructItem<IntVec3>() { Name = "Block Position", GetValue = LauncherWrapper.GetHitBlockPosition, Description = "Grid location of the block the player is looking at" });
                    Items.Add(new EnumItem<BlockFace>() { Name = "Block Face", GetValue = LauncherWrapper.GetHitFace, Description = "Which side of the block the player is looking at" });
                    break;
                case HitResult.Entity:
                    //Items = entityStats;
                    Items.Add(new EnumItem<EntityRenderModel>() { Name = "Type", GetValue = LauncherWrapper.GetEntityRenderModel, Description = "What type the entity is" });
                    Items.Add(new PrimitiveItem<Vec3>() { Name = "Position", GetValue = LauncherWrapper.GetEntityPosition, Description = "The location of the entity" });
                    Items.Add(new PrimitiveItem<Vec3>() { Name = "Velocity", GetValue = LauncherWrapper.GetEntityVelocity, Description = "How fast the entity is moving" });
                    Items.Add(new PrimitiveItem<Vec2>() { Name = "Rotation", GetValue = LauncherWrapper.GetEntityRotation, Description = "The rotation of the entity's body" });
                    Items.Add(new PrimitiveItem<int>() { Name = "Age", GetValue = LauncherWrapper.GetEntityAge, Description = "How old the entity is (measured in game ticks)" });
                    Items.Add(new PrimitiveItem<float>() { Name = "Distance Travelled", GetValue = LauncherWrapper.GetEntityDistanceTravelled, Description = "How far the entity has travelled since joining the world" });

                    EntityRenderModel type = LauncherWrapper.GetEntityRenderModel();

                    IntVec3 color = LauncherWrapper.GetEntityColor();
                    if (color.x + color.y + color.z > 0)
                        Items.Add(new ColorItem() { Name = "Color", GetValue = LauncherWrapper.GetEntityColor, Description = "The color of the entity" });

                    // Horribly Slow
                    if (EntityUtils.isMob(type))
                    {
                        int totalAttributes = LauncherWrapper.GetHitEntityAttributesLength();
                        if (totalAttributes > 0)
                        {
                            CompoundItem attributeStats = new CompoundItem() { Name = "Attributes" };
                            {
                                for (int ind = 0; ind < totalAttributes; ind++)
                                {
                                    StringBuilder attributeName = new StringBuilder(31);

                                    int length = LauncherWrapper.GetHitEntityAttributeName(ind, attributeName);
                                    string name = attributeName.ToString();
                                    if (String.IsNullOrEmpty(name))
                                        continue;

                                    int strInd = name.IndexOf(".") + 1;
                                    name = name.Substring(strInd, length - strInd);
                                    name = char.ToUpper(name[0]) + name.Substring(1);

                                    attributeStats.Items.Add(new AttributeItem() { Name = name, Index = ind, GetValue = LauncherWrapper.GetHitEntityAttributValues, Description = EntityUtils.getAttributeDescription(name) });
                                }
                            }
                            Items.Add(attributeStats);
                        }
                    }

                    break;
                default:
                    break;
            }
        }
    }
}