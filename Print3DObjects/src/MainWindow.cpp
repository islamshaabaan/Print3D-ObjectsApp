#include <MainWindow.h>
#include "ui_MainWindow.h"


double MainWindow::azimuth = 0;
double MainWindow::elevation = 0;

/// <summary>
///     Constructor of the MainWindow Class.
/// </summary>
/// <param name="parent"></param>
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    mRenderWindow(vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New()),
    mRenderer(vtkSmartPointer<vtkRenderer>::New()),
    mInteractor(vtkSmartPointer<QVTKInteractor>::New()),
    mInteractorStyle1(vtkSmartPointer < vtkInteractorStyleTrackballActor>::New()),
    picker (vtkSmartPointer<vtkCellPicker>::New()),
    mInteractorStyle(vtkSmartPointer < vtkInteractorStyle>::New())
{
    ui->setupUi(this);
    organizingWindowDependecies();
    InitializeBackground();
    InitializeSlotsAndSignals();
    InitializeCameraSettings();

    // Render and interact
    ui->openGLWidget->renderWindow()->SetWindowName("Paint3D | MedSoft");

}

/// <summary>
///     Destructor of the MainWindow Class.
/// </summary>
MainWindow::~MainWindow()
{
    delete ui;
}

/// <summary>
///     Private Method of MainWindow Class
///     To Initialize the slots with the signals.
/// </summary>
void MainWindow::InitializeSlotsAndSignals() {
    //Set the ui connection
    QObject::connect(ui->drawSphere_button, &QPushButton::clicked,
        this, &MainWindow::onDrawSphereClick);
    QObject::connect(ui->drawCube_button, &QPushButton::clicked,
        this, &MainWindow::onDrawCubeClick);
    QObject::connect(ui->drawCone_button, &QPushButton::clicked,
        this, &MainWindow::onDrawConeClick);
    QObject::connect(ui->drawPyramid_button, &QPushButton::clicked,
        this, &MainWindow::onDrawPyramidClick);
    QObject::connect(ui->drawCylinder_button, &QPushButton::clicked,
        this, &MainWindow::onDrawCylinderClick);
    QObject::connect(ui->drawTube_button, &QPushButton::clicked,
        this, &MainWindow::onDrawTubeClick);
    QObject::connect(ui->drawDoughnut_button, &QPushButton::clicked,
        this, &MainWindow::onDrawDoughnutClick);
    QObject::connect(ui->drawCurvedCylinder_button, &QPushButton::clicked,
        this, &MainWindow::onDrawCurvedCylinderClick);
    QObject::connect(ui->drawSemiSphere_button, &QPushButton::clicked,
        this, &MainWindow::onDrawSemiSphereClick);

    QObject::connect(ui->cameraX, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this, &MainWindow::updateCameraX);
    QObject::connect(ui->cameraY, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this, &MainWindow::updateCameraY);
    QObject::connect(ui->cameraZ, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this, &MainWindow::updateCameraZ);
    QObject::connect(ui->Elevation, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this, &MainWindow::updateCameraAzimuth);
    QObject::connect(ui->Azimuth, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this, &MainWindow::updateCameraElevation);
    QObject::connect(ui->clearSpace_button, &QPushButton::clicked, 
        this, &MainWindow::newSketch);
    QObject::connect(ui->deleteActor_button, &QPushButton::clicked, 
        this, &MainWindow::deleteActor);


}

/// <summary>
///     Private Method of MainWindow Class
///     To Setup VTK components.
/// </summary>
void MainWindow::organizingWindowDependecies()
{
    ui->openGLWidget->setRenderWindow(mRenderWindow);
    mRenderWindow->AddRenderer(mRenderer);
    mRenderWindow->SetInteractor(mInteractor);
    mInteractorStyle1->SetDefaultRenderer(mRenderer);
    mInteractor->SetRenderWindow(mRenderWindow);
    mInteractor->SetInteractorStyle(mInteractorStyle1);
    mInteractor->Initialize();
}

/// <summary>
///     Private Method of MainWindow Class
///     To Set the background color of the renderer.
/// </summary>
void MainWindow::InitializeBackground()
{
    //Set the background color
    mRenderer->SetBackground(colors->GetColor3d("SteelBlue").GetData());
}

/// <summary>
///     Private Method of MainWindow Class
///     To Initialize the Camera setting.
/// </summary>
void MainWindow::InitializeCameraSettings()
{
    mRenderer->GetActiveCamera()->SetPosition(0, 0, 5);
    mRenderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
}

/// <summary>
///     Private Method of MainWindow Class
///     To delete all the Actors(Objects) from the renderer.
/// </summary>
void MainWindow::newSketch()
{
    mRenderer->RemoveAllViewProps();
    InitializeCameraSettings();
    mRenderWindow->Render();
}

/// <summary>
///     Private Method of MainWindow Class
///     To Pick the Selected Actor by the mouse.
/// </summary>
/// <returns>
///     The Selected Actor.
/// </returns>
vtkActor* MainWindow::pickActor() {
    mRenderWindow->GetInteractor()->SetPicker(picker);
    picker->SetTolerance(0.001);
    picker->Pick(mRenderWindow->GetInteractor()->GetEventPosition()[0],
        mRenderWindow->GetInteractor()->GetEventPosition()[1],
        0,
        mRenderer);
    vtkActor* actor = picker->GetActor();
    if (!actors.isEmpty() && actor) {
        QMessageBox::information(this, "Information", "Actor is selected");
        return actor;
    }
    QMessageBox::warning(this, "warning", "No Actors is selected");
    return nullptr;
}

/// <summary>
///     public Method of MainWindow Class
///     To Delete the Selected Actor by the mouse.
/// </summary>
void MainWindow::deleteActor()
{
    vtkActor* selectedActor = pickActor();
    if (selectedActor) {
        mRenderer->RemoveActor(selectedActor);
        actors.removeOne(selectedActor);
        mRenderWindow->Render();
    }
}

/// <summary>
///     Private Method of MainWindow Class
///     Update camera settings spinners.
/// </summary>
void MainWindow::updatePosition() {
    double* position = mRenderer->GetActiveCamera()->GetPosition();
    ui->cameraX->setValue(position[0]);
    ui->cameraY->setValue(position[1]);
    ui->cameraZ->setValue(position[2]);
}

/// <summary>
///     Public Method of MainWindow Class
///     Add a given value to the camera in the X-Axis.
/// </summary>
/// <param name="value">
///     double Type.
/// </param>
void MainWindow::updateCameraX(double value) {
    double* position = mRenderer->GetActiveCamera()->GetPosition();
    mRenderer->GetActiveCamera()->SetPosition(value, position[1], position[2]);
    mRenderWindow->Render();
}

/// <summary>
///     Public Method of MainWindow Class
///     Add a given value to the camera in the Y-Axis.
/// </summary>
/// <param name="value">
///     double Type.
/// </param>
void MainWindow::updateCameraY(double value) {
    double* position = mRenderer->GetActiveCamera()->GetPosition();
    mRenderer->GetActiveCamera()->SetPosition(position[0], value, position[2]);
    mRenderWindow->Render();
}

/// <summary>
///     Public Method of MainWindow Class
///     Add a given value to the camera in the Z-Axis.
/// </summary>
/// <param name="value">
///     double Type.
/// </param>
void MainWindow::updateCameraZ(double value) {
    double* position = mRenderer->GetActiveCamera()->GetPosition();
    mRenderer->GetActiveCamera()->SetPosition(position[0], position[1], value);
    mRenderWindow->Render();
}

/// <summary>
///     Public Method of MainWindow Class
///     Rotate the camera about the view up vector centered at the focal point.
/// </summary>
/// <param name="value">
///     double Type.
/// </param>
void MainWindow::updateCameraAzimuth(double value) {
    mRenderer->GetActiveCamera()->Azimuth(-azimuth);
    mRenderer->GetActiveCamera()->Azimuth(value);
    azimuth = value;
    updatePosition();
    mRenderWindow->Render();
}

/// <summary>
///     Public Method of MainWindow Class
///     Rotate the camera about the cross product of the negative of the direction of projection 
///		and the view up vector, using the focal point as the center of rotation.
/// </summary>
/// <param name="value">
///     double type
/// </param>
void MainWindow::updateCameraElevation(double value) {
    mRenderer->GetActiveCamera()->Elevation(-elevation);
    mRenderer->GetActiveCamera()->Elevation(value);
    elevation = value;
    updatePosition();
    mRenderWindow->Render();
}

/// <summary>
///     Private Method of MainWindow Class
///     To Save the Project as an Image File.
/// </summary>
void MainWindow::on_actionSave_triggered()
{
    QImage img = ui->openGLWidget->grabFramebuffer();
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image File"),
        QString(),
        tr("Images (*.png)"));
    if (!fileName.isEmpty())
    {
        img.save(fileName);
    }
}

/// <summary>
///     Private Method of MainWindow Class
///     To Close the MainWindow.
/// </summary>
void MainWindow::on_actionClose_triggered()
{
    this->close();
}

/// <summary>
///     Public Method of MainWindow Class
///     Draw a Sphere Object on the OpenGLWidget.
/// </summary>
void MainWindow::onDrawSphereClick() {

    //Setup VTK components
    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();

    sphereSource->SetRadius(2);
    sphereSource->Update();

    sphereMapper->SetInputData(sphereSource->GetOutput());

    sphereActor->SetMapper(sphereMapper);

    // Pick Color
    QString colorVariant = ui->colorComboBox->currentText();
    vtkStdString colorStd = colorVariant.toStdString();
    sphereActor->GetProperty()->SetColor(colors->GetColor4d(colorStd).GetData());

    mRenderer->AddViewProp(sphereActor);
    mRenderer->ResetCamera();
    updatePosition();

    mRenderWindow->AddRenderer(mRenderer);

    mRenderWindow->GetInteractor()->SetInteractorStyle(mInteractorStyle1);
    mRenderWindow->GetInteractor()->SetRenderWindow(mRenderWindow);
    mRenderWindow->GetInteractor()->SetPicker(picker);

    mRenderWindow->Render();


    actors.append(sphereActor);
}

/// <summary>
///     Public Method of MainWindow Class
///     Draw a Cube Object on the OpenGLWidget.
/// </summary>
void MainWindow::onDrawCubeClick() {
    //Create the cube source and mapper and actor
    vtkSmartPointer<vtkCubeSource> cubeSource =vtkSmartPointer<vtkCubeSource>::New();
    vtkSmartPointer<vtkPolyDataMapper> cubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkActor> cubeActor = vtkSmartPointer<vtkActor>::New();

    cubeSource->SetXLength(3);
    cubeSource->SetYLength(2);
    cubeSource->SetZLength(3);
    cubeSource->Update();

    cubeMapper->SetInputData(cubeSource->GetOutput());

    cubeActor->SetMapper(cubeMapper);

    // pick the color from the combo box
    QString colorVariant = ui->colorComboBox->currentText();
    vtkStdString colorStd = colorVariant.toStdString();
    cubeActor->GetProperty()->SetColor(colors->GetColor4d(colorStd).GetData());

    //cubeActor->SetPosition(0, 0, -10);
    mRenderer->AddActor(cubeActor);
    mRenderer->ResetCamera();
    updatePosition();

    mRenderWindow->AddRenderer(mRenderer);

    //renderWindowInterActor->SetInteractorStyle(mInteractorStyle1);
    mRenderWindow->GetInteractor()->SetInteractorStyle(mInteractorStyle1);

    //renderWindowInterActor->SetPicker(picker);
    mRenderWindow->GetInteractor()->SetPicker(picker);

    mRenderWindow->Render();
    //mRenderWindow->GetInteractor()->Start();
    //renderWindowInterActor->Start();
    actors.append(cubeActor);

}

/// <summary>
///     Public Method of MainWindow Class
///     Draw a Cylinder Object on the OpenGLWidget.
/// </summary>
void MainWindow::onDrawCylinderClick() {

    //Create the cylinder source and mapper and actor
    vtkSmartPointer<vtkCylinderSource> cylinderSource = vtkSmartPointer<vtkCylinderSource>::New();
    vtkSmartPointer<vtkPolyDataMapper> cylinderMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkActor> cylinderActor = vtkSmartPointer<vtkActor>::New();

    cylinderSource->SetRadius(2);
    cylinderSource->SetHeight(3);
    cylinderSource->Update();

    cylinderMapper->SetInputData(cylinderSource->GetOutput());

    cylinderActor->SetMapper(cylinderMapper);

    // pick the color from the combo box
    QString colorVariant = ui->colorComboBox->currentText();
    vtkStdString colorStd = colorVariant.toStdString();
    cylinderActor->GetProperty()->SetColor(colors->GetColor4d(colorStd).GetData());

    mRenderer->AddActor(cylinderActor);
    mRenderer->ResetCamera();
    updatePosition();

    mRenderWindow->AddRenderer(mRenderer);

    mRenderWindow->GetInteractor()->SetInteractorStyle(mInteractorStyle1);

    mRenderWindow->GetInteractor()->SetPicker(picker);

    mRenderWindow->Render();
  
    actors.append(cylinderActor);

}

/// <summary>
///     Public Method of MainWindow Class
///     Draw a Cone Object on the OpenGLWidget.
/// </summary>
void MainWindow::onDrawConeClick() {
    // Cone
    vtkSmartPointer<vtkConeSource> coneSource = vtkSmartPointer<vtkConeSource>::New();
    vtkSmartPointer<vtkPolyDataMapper> coneMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkActor> coneActor = vtkSmartPointer<vtkActor>::New();

    coneSource->SetAngle(50);
    coneSource->SetHeight(3);
    coneSource->SetRadius(2);
    coneSource->Update();

    coneMapper->SetInputData(coneSource->GetOutput());

    coneActor->SetMapper(coneMapper);

    // pick the color from the combo box
    QString colorVariant = ui->colorComboBox->currentText();
    vtkStdString colorStd = colorVariant.toStdString();
    coneActor->GetProperty()->SetColor(colors->GetColor4d(colorStd).GetData());

    mRenderer->AddActor(coneActor);
    mRenderer->ResetCamera();
    updatePosition();

    mRenderWindow->AddRenderer(mRenderer);

    mRenderWindow->GetInteractor()->SetInteractorStyle(mInteractorStyle1);

    mRenderWindow->GetInteractor()->SetPicker(picker);

    mRenderWindow->Render();

    actors.append(coneActor);

}

/// <summary>
///     Public Method of MainWindow Class
///     Draw a Pyramid Object with 4Faces on the OpenGLWidget.
/// </summary>
void MainWindow::onDrawPyramidClick() {  
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(0, 0, 0); // Base vertex
    points->InsertNextPoint(1, 0, 0); // Base vertex
    points->InsertNextPoint(1, 1, 0); // Base vertex
    points->InsertNextPoint(0, 1, 0); // Base vertex
    points->InsertNextPoint(0.5, 0.5, 1); // Apex

    vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
    cells->InsertNextCell(4); // 4 Vertices in the base
    cells->InsertCellPoint(0);
    cells->InsertCellPoint(1);
    cells->InsertCellPoint(2);
    cells->InsertCellPoint(3);

    cells->InsertNextCell(3); // 3 Vertices in the base
    cells->InsertCellPoint(0);
    cells->InsertCellPoint(1);
    cells->InsertCellPoint(4);


    cells->InsertNextCell(3); // Repeat
    cells->InsertCellPoint(1);
    cells->InsertCellPoint(2);
    cells->InsertCellPoint(4);

    cells->InsertNextCell(3); // Repeat
    cells->InsertCellPoint(2);
    cells->InsertCellPoint(3);
    cells->InsertCellPoint(4);

    cells->InsertNextCell(3); // Repeat
    cells->InsertCellPoint(3);
    cells->InsertCellPoint(0);
    cells->InsertCellPoint(4);


    vtkSmartPointer<vtkPolyData> pyramid = vtkSmartPointer<vtkPolyData>::New();
    pyramid->SetPoints(points);
    pyramid->SetPolys(cells);

    vtkSmartPointer<vtkPolyDataMapper> pyramidMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkActor> pyramidActor = vtkSmartPointer<vtkActor>::New();

    pyramidMapper->SetInputData(pyramid);

    pyramidActor->SetMapper(pyramidMapper);

    // pick the color from the combo box
    QString colorVariant = ui->colorComboBox->currentText();
    vtkStdString colorStd = colorVariant.toStdString();
    pyramidActor->GetProperty()->SetColor(colors->GetColor4d(colorStd).GetData());

    mRenderer->AddActor(pyramidActor);
    mRenderer->ResetCamera();
    updatePosition();

    mRenderWindow->AddRenderer(mRenderer);

    mRenderWindow->GetInteractor()->SetInteractorStyle(mInteractorStyle1);

    mRenderWindow->GetInteractor()->SetPicker(picker);

    mRenderWindow->Render();

    actors.append(pyramidActor);

}

/// <summary>
///     Public Method of MainWindow Class
///     Draw a Tube Object on the OpenGLWidget.
/// </summary>
void MainWindow::onDrawTubeClick() {
    //Create a line source
    //vtkSmartPointer<vtkLineSource> lineSource = vtkSmartPointer<vtkLineSource>::New();
    //lineSource->SetPoint1(0, 0, 0);
    //lineSource->SetPoint2(1, 0, 0);
    // Create a parametric spline curve using vtkParametricSpline
    vtkSmartPointer<vtkParametricSpline> spline = vtkSmartPointer<vtkParametricSpline>::New();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    // Set up control points for the spline
    points->InsertNextPoint(0, 0, 0);
    points->InsertNextPoint(1, 0.5, 0);
    points->InsertNextPoint(2, 1, 0);
    spline->SetPoints(points);

    // Create a parametric function source using vtkParametricFunctionSource
    vtkSmartPointer<vtkParametricFunctionSource> functionSource =
        vtkSmartPointer<vtkParametricFunctionSource>::New();
    functionSource->SetParametricFunction(spline);
    functionSource->SetUResolution(100); // Number of points along the curve

    //Create a tube source
    vtkSmartPointer<vtkTubeFilter> tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
    tubeFilter->SetInputConnection(functionSource->GetOutputPort());
    tubeFilter->SetRadius(0.1);

    //Mapper and Actor
    vtkSmartPointer<vtkPolyDataMapper> tubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkActor> tubeActor = vtkSmartPointer<vtkActor>::New();
    tubeMapper->SetInputConnection(tubeFilter->GetOutputPort());
    tubeActor->SetMapper(tubeMapper);

    // pick the color from the combo box
    QString colorVariant = ui->colorComboBox->currentText();
    vtkStdString colorStd = colorVariant.toStdString();
    tubeActor->GetProperty()->SetColor(colors->GetColor4d(colorStd).GetData());

    mRenderer->AddActor(tubeActor);
    mRenderer->ResetCamera();
    updatePosition();

    mRenderWindow->AddRenderer(mRenderer);

    mRenderWindow->GetInteractor()->SetInteractorStyle(mInteractorStyle1);

    mRenderWindow->GetInteractor()->SetPicker(picker);

    mRenderWindow->Render();

    actors.append(tubeActor);

}

/// <summary>
///     Public Method of MainWindow Class
///     Draw a Doughnut Object on the OpenGLWidget.
/// </summary>
void MainWindow::onDrawDoughnutClick() {

    //Create the cylinder source and mapper and actor
    vtkSmartPointer<vtkParametricTorus> outerTorusSource = vtkSmartPointer<vtkParametricTorus>::New();
    vtkSmartPointer<vtkParametricTorus> innerTorusSource = vtkSmartPointer<vtkParametricTorus>::New();

    vtkSmartPointer<vtkPolyDataMapper> doughnutMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkActor> doughnutActor = vtkSmartPointer<vtkActor>::New();

    outerTorusSource->SetRingRadius(2);
    outerTorusSource->SetCrossSectionRadius(0.5);

    innerTorusSource->SetRingRadius(1.5);
    innerTorusSource->SetCrossSectionRadius(0.2);

    //create Function sources for outer and inner
    vtkSmartPointer<vtkParametricFunctionSource> outerSource = vtkSmartPointer<vtkParametricFunctionSource>::New();
    outerSource->SetParametricFunction(outerTorusSource);
    outerSource->SetUResolution(50);
    outerSource->SetVResolution(50);
    outerSource->SetWResolution(50);
    outerSource->Update();

    vtkSmartPointer<vtkParametricFunctionSource> innerSource = vtkSmartPointer<vtkParametricFunctionSource>::New();
    innerSource->SetParametricFunction(innerTorusSource);
    innerSource->SetUResolution(50);
    innerSource->SetVResolution(50);
    innerSource->SetWResolution(50);
    innerSource->Update();

    //Apply Transformation to position the inner with the outer
    vtkSmartPointer<vtkTransform> Transform = vtkSmartPointer<vtkTransform>::New();
    Transform->Translate(0, 0, 0);

    vtkSmartPointer<vtkTransformPolyDataFilter> TransformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    TransformFilter->SetInputConnection(innerSource->GetOutputPort());
    TransformFilter->SetTransform(Transform);
    TransformFilter->Update();

    //Combine the 2 tubes to create the doughnut
    vtkSmartPointer <vtkAppendPolyData> appendFilter = vtkSmartPointer <vtkAppendPolyData>::New();
    appendFilter->AddInputData(outerSource->GetOutput());
    appendFilter->AddInputData(TransformFilter->GetOutput());
    appendFilter->Update();

    //Mapper and Actor
    doughnutMapper->SetInputData(appendFilter->GetOutput());

    doughnutActor->SetMapper(doughnutMapper);

    // pick the color from the combo box
    QString colorVariant = ui->colorComboBox->currentText();
    vtkStdString colorStd = colorVariant.toStdString();
    doughnutActor->GetProperty()->SetColor(colors->GetColor4d(colorStd).GetData());

    mRenderer->AddActor(doughnutActor);
    mRenderer->ResetCamera();
    updatePosition();

    mRenderWindow->AddRenderer(mRenderer);

    mRenderWindow->GetInteractor()->SetInteractorStyle(mInteractorStyle1);

    mRenderWindow->GetInteractor()->SetPicker(picker);

    mRenderWindow->Render();

    actors.append(doughnutActor);

}

/// <summary>
///     Public Method of MainWindow Class
///     Draw a Curved Cylinder Object on the OpenGLWidget.
/// </summary>
void MainWindow::onDrawCurvedCylinderClick() {
    
    /*vtkSmartPointer<vtkCylinderSource> cylinderSource = vtkSmartPointer<vtkCylinderSource>::New();
    cylinderSource->SetRadius(0.2);
    cylinderSource->SetHeight(3);
    cylinderSource->SetResolution(1000);
    */
    
    vtkSmartPointer<vtkParametricSpline> splineFunction = vtkSmartPointer<vtkParametricSpline>::New();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    for (int i = 0; i < 100; i++) {
        double t = 2.0 * vtkMath::Pi() * i / (100 - 1);
        double x = cos(t);
        double y = sin(t);
        double z = 0.1 * t; // the curvature
        points->InsertNextPoint(x, y, z);

    }
    /*points->InsertNextPoint(0, 0, 0);
    points->InsertNextPoint(1, 0, 2); 
    points->InsertNextPoint(2, 0, 4); 
    points->InsertNextPoint(3, 0, 2); 
    points->InsertNextPoint(4, 0, 0); */
    splineFunction->SetPoints(points);


    vtkSmartPointer<vtkParametricFunctionSource> ParamFunSource = vtkSmartPointer<vtkParametricFunctionSource>::New();
    ParamFunSource->SetParametricFunction(splineFunction);
    ParamFunSource->SetUResolution(100);
    ParamFunSource->SetVResolution(30);
    ParamFunSource->Update();

    /*vtkSmartPointer<vtkTransform> Transform = vtkSmartPointer<vtkTransform>::New();
    Transform->RotateWXYZ(45, 0, 1, 0);
    vtkSmartPointer<vtkTransformPolyDataFilter> TransformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    TransformFilter->SetInputConnection(cylinderSource->GetOutputPort());
    TransformFilter->SetTransform(Transform);
    TransformFilter->Update();
    
   vtkSmartPointer<vtkMatrix4x4> matrix = vtkSmartPointer<vtkMatrix4x4>::New();
    matrix->Identity();
    for (int i = 0; i < ParamFunSource->GetOutput()->GetPoints()->GetNumberOfPoints(); ++i) {
        double *point;
        ParamFunSource->GetOutput()->GetPoints()->GetPoint(i, point);
        matrix->SetElement(i, 3, *point);
    }
    Transform->Concatenate(matrix);*/
    
    vtkSmartPointer<vtkPolyDataMapper> curvedCylinderMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkActor> curvedCylinderActor = vtkSmartPointer<vtkActor>::New();

    //Mapper and Actor
    curvedCylinderMapper->SetInputData(ParamFunSource->GetOutput());

    curvedCylinderActor->SetMapper(curvedCylinderMapper);

    // pick the color from the combo box
    QString colorVariant = ui->colorComboBox->currentText();
    vtkStdString colorStd = colorVariant.toStdString();
    curvedCylinderActor->GetProperty()->SetColor(colors->GetColor4d(colorStd).GetData());

    mRenderer->AddActor(curvedCylinderActor);
    mRenderer->ResetCamera();
    updatePosition();

    mRenderWindow->AddRenderer(mRenderer);

    mRenderWindow->GetInteractor()->SetInteractorStyle(mInteractorStyle1);

    mRenderWindow->GetInteractor()->SetPicker(picker);

    mRenderWindow->Render();

    actors.append(curvedCylinderActor);

}

/// <summary>
///     Public Method of MainWindow Class
///     Draw a Hemi Sphere Object on the OpenGLWidget.
/// </summary>
void MainWindow::onDrawSemiSphereClick(){

    //Setup VTK components
    vtkSmartPointer<vtkSphereSource> HemiSphereSource = vtkSmartPointer<vtkSphereSource>::New();
    vtkSmartPointer<vtkPolyDataMapper> HemiSphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkActor> HemiSphereActor = vtkSmartPointer<vtkActor>::New();

    HemiSphereSource->SetPhiResolution(30);
    HemiSphereSource->SetThetaResolution(30);
    HemiSphereSource->SetRadius(1.0);
    HemiSphereSource->SetCenter(0.0, 0.0, 0.0);
    HemiSphereSource->SetStartTheta(0); // Start the sphere from the equator
    HemiSphereSource->SetEndTheta(180); // End the sphere at the pole
    HemiSphereSource->Update();

    HemiSphereMapper->SetInputData(HemiSphereSource->GetOutput());

    HemiSphereActor->SetMapper(HemiSphereMapper);

    // Pick Color
    QString colorVariant = ui->colorComboBox->currentText();
    vtkStdString colorStd = colorVariant.toStdString();
    HemiSphereActor->GetProperty()->SetColor(colors->GetColor4d(colorStd).GetData());

    mRenderer->AddViewProp(HemiSphereActor);
    mRenderer->ResetCamera();
    updatePosition();

    mRenderWindow->AddRenderer(mRenderer);

    mRenderWindow->GetInteractor()->SetInteractorStyle(mInteractorStyle1);
    mRenderWindow->GetInteractor()->SetRenderWindow(mRenderWindow);
    mRenderWindow->GetInteractor()->SetPicker(picker);

    mRenderWindow->Render();


    actors.append(HemiSphereActor);

}
