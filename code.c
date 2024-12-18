#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Structure to hold the entries for the doctor registration
typedef struct {
    GtkWidget *name_entry;
    GtkWidget *age_entry;
    GtkWidget *specialization_entry;
    GtkWidget *contact_entry;
    GtkWidget *experience_entry;
    GtkWidget *fees_entry;
} DoctorEntries;

// Structure to hold the entries for the appointment
typedef struct {
    GtkWidget *patient_name_entry;
    GtkWidget *patient_age_entry;
    GtkWidget *patient_contact_entry;
    GtkWidget *appointment_date_entry;
    GtkWidget *doctor_name_entry;
} AppointmentEntries;

// Global arrays to hold doctor and patient data
typedef struct {
    char name[100];
    char age[10];
    char contact[15];
    char specialization[100];
    char experience[10];
    char fees[10];
} Doctor;

typedef struct {
    char patient_name[100];
    char patient_age[10];
    char patient_contact[15];
    char appointment_date[20];
    char doctor_name[100];
} Appointment;

Doctor doctors[100];
Appointment appointments[100];
int doctor_count = 0;
int appointment_count = 0;

// Forward declarations of functions
void create_main_menu();
static void on_take_appointment_button_clicked(GtkButton *button, gpointer user_data);
static void on_register_doctor_button_clicked(GtkButton *button, gpointer user_data);
static void on_doctor_register(GtkButton *button, gpointer user_data);
static void on_view_patient_history_button_clicked(GtkButton *button, gpointer user_data);
static void on_appointment_register(GtkButton *button, gpointer user_data);
static void on_login_button_clicked(GtkButton *button, gpointer user_data);

// Function to handle Login button click
static void on_login_button_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget **entries = (GtkWidget **)user_data;  // Cast user_data back to the entries array
    GtkWidget *username_entry = entries[0];
    GtkWidget *password_entry = entries[1];
    GtkWidget *error_label = entries[2];
    
    // Get the entered username and password
    const char *username = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const char *password = gtk_entry_get_text(GTK_ENTRY(password_entry));

    printf("Entered Username: %s\n", username);
    printf("Entered Password: %s\n", password);

    // Case-insensitive dummy login validation
    if (strcasecmp(username, "admin") == 0 && strcasecmp(password, "password") == 0) {
        gtk_widget_hide(GTK_WIDGET(entries[3]));  // Hide login page
        create_main_menu();  // Show the main menu after successful login
    } else {
        gtk_label_set_text(GTK_LABEL(error_label), "Invalid login details");
    }
}

// Function to create the main menu after login
void create_main_menu() {
    GtkWidget *window, *grid, *appointment_button, *register_doctor_button, *view_patient_button;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Main Menu");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Take appointment button
    appointment_button = gtk_button_new_with_label("Take Appointment");
    gtk_grid_attach(GTK_GRID(grid), appointment_button, 0, 0, 1, 1);

    // Register doctor button
    register_doctor_button = gtk_button_new_with_label("Register Doctor");
    gtk_grid_attach(GTK_GRID(grid), register_doctor_button, 0, 1, 1, 1);

    // View patient history button
    view_patient_button = gtk_button_new_with_label("View Patient History");
    gtk_grid_attach(GTK_GRID(grid), view_patient_button, 0, 2, 1, 1);

    // Connect buttons to respective functions
    g_signal_connect(appointment_button, "clicked", G_CALLBACK(on_take_appointment_button_clicked), NULL);
    g_signal_connect(register_doctor_button, "clicked", G_CALLBACK(on_register_doctor_button_clicked), NULL);
    g_signal_connect(view_patient_button, "clicked", G_CALLBACK(on_view_patient_history_button_clicked), NULL);

    gtk_widget_show_all(window);
}

// Function for "View Patient History" functionality
static void on_view_patient_history_button_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *window, *grid, *history_label;

    // Create a new window to display the patient history
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Patient History");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_widget_destroy), NULL);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Build the patient history string with proper formatting
    char history_text[1000] = "Patient History:\n\n";  // Add extra space between title and history

    for (int i = 0; i < appointment_count; i++) {
        char appointment_details[300];
        sprintf(appointment_details, 
                "Patient: %s\nAge: %s\nContact: %s\nDate: %s\nDoctor: %s\n\n",  // Added newlines between details
                appointments[i].patient_name, appointments[i].patient_age, 
                appointments[i].patient_contact, appointments[i].appointment_date, appointments[i].doctor_name);
        strcat(history_text, appointment_details);
    }

    // Create a GtkLabel to display the patient history
    history_label = gtk_label_new(history_text);
    gtk_label_set_xalign(GTK_LABEL(history_label), 0.0);  // Align text to the left for readability
    gtk_grid_attach(GTK_GRID(grid), history_label, 0, 0, 1, 1);

    gtk_widget_show_all(window);
}

// Function for "Take Appointment" functionality
static void on_take_appointment_button_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *window, *grid, *patient_name_entry, *patient_age_entry, *patient_contact_entry, *appointment_date_entry, *doctor_name_entry, *register_button;

    // Create a new window for appointment
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Take Appointment");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_widget_destroy), NULL);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Patient name entry
    patient_name_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Patient Name:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), patient_name_entry, 1, 0, 2, 1);

    // Patient age entry
    patient_age_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Patient Age:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), patient_age_entry, 1, 1, 2, 1);

    // Patient contact entry
    patient_contact_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Patient Contact:"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), patient_contact_entry, 1, 2, 2, 1);

    // Appointment date entry
    appointment_date_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Appointment Date:"), 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), appointment_date_entry, 1, 3, 2, 1);

    // Doctor name entry
    doctor_name_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Doctor Name:"), 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), doctor_name_entry, 1, 4, 2, 1);

    // Register button to confirm appointment
    register_button = gtk_button_new_with_label("Confirm Appointment");
    gtk_grid_attach(GTK_GRID(grid), register_button, 0, 5, 3, 1);

    // Create the struct to hold the appointment details
    AppointmentEntries *appointment_entries = g_malloc(sizeof(AppointmentEntries));
    appointment_entries->patient_name_entry = patient_name_entry;
    appointment_entries->patient_age_entry = patient_age_entry;
    appointment_entries->patient_contact_entry = patient_contact_entry;
    appointment_entries->appointment_date_entry = appointment_date_entry;
    appointment_entries->doctor_name_entry = doctor_name_entry;

    // Connect the button to the appointment registration function
    g_signal_connect(register_button, "clicked", G_CALLBACK(on_appointment_register), appointment_entries);

    gtk_widget_show_all(window);
}

// Function for registering the appointment
static void on_appointment_register(GtkButton *button, gpointer user_data) {
    AppointmentEntries *entries = (AppointmentEntries *)user_data;

    const char *patient_name = gtk_entry_get_text(GTK_ENTRY(entries->patient_name_entry));
    const char *patient_age = gtk_entry_get_text(GTK_ENTRY(entries->patient_age_entry));
    const char *patient_contact = gtk_entry_get_text(GTK_ENTRY(entries->patient_contact_entry));
    const char *appointment_date = gtk_entry_get_text(GTK_ENTRY(entries->appointment_date_entry));
    const char *doctor_name = gtk_entry_get_text(GTK_ENTRY(entries->doctor_name_entry));

    printf("Appointment Details:\n");
    printf("Patient Name: %s\n", patient_name);
    printf("Patient Age: %s\n", patient_age);
    printf("Patient Contact: %s\n", patient_contact);
    printf("Appointment Date: %s\n", appointment_date);
    printf("Doctor Name: %s\n", doctor_name);

    // Save the appointment data
    strcpy(appointments[appointment_count].patient_name, patient_name);
    strcpy(appointments[appointment_count].patient_age, patient_age);
    strcpy(appointments[appointment_count].patient_contact, patient_contact);
    strcpy(appointments[appointment_count].appointment_date, appointment_date);
    strcpy(appointments[appointment_count].doctor_name, doctor_name);
    appointment_count++;

    // Show confirmation message
    GtkWidget *message_dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Appointment Confirmed!");
    gtk_dialog_run(GTK_DIALOG(message_dialog));
    gtk_widget_destroy(message_dialog);

    // Free the allocated memory for the entries structure
    g_free(entries);
}

// Function for "Register Doctor" functionality
static void on_register_doctor_button_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *window, *grid, *name_entry, *age_entry, *specialization_entry, *contact_entry, *experience_entry, *fees_entry, *register_button;
    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Register Doctor");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_widget_destroy), NULL);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Doctor name entry
    name_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Doctor Name:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 0, 2, 1);

    // Doctor age entry
    age_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Doctor Age:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), age_entry, 1, 1, 2, 1);

    // Doctor specialization entry
    specialization_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Specialization:"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), specialization_entry, 1, 2, 2, 1);

    // Doctor contact entry
    contact_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Doctor Contact:"), 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), contact_entry, 1, 3, 2, 1);

    // Doctor experience entry
    experience_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Experience:"), 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), experience_entry, 1, 4, 2, 1);

    // Doctor fees entry
    fees_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Fees:"), 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), fees_entry, 1, 5, 2, 1);

    // Register button
    register_button = gtk_button_new_with_label("Register Doctor");
    gtk_grid_attach(GTK_GRID(grid), register_button, 0, 6, 3, 1);

    DoctorEntries *doctor_entries = g_malloc(sizeof(DoctorEntries));
    doctor_entries->name_entry = name_entry;
    doctor_entries->age_entry = age_entry;
    doctor_entries->specialization_entry = specialization_entry;
    doctor_entries->contact_entry = contact_entry;
    doctor_entries->experience_entry = experience_entry;
    doctor_entries->fees_entry = fees_entry;

    g_signal_connect(register_button, "clicked", G_CALLBACK(on_doctor_register), doctor_entries);

    gtk_widget_show_all(window);
}

// Function to handle doctor registration
static void on_doctor_register(GtkButton *button, gpointer user_data) {
    DoctorEntries *entries = (DoctorEntries *)user_data;

    const char *doctor_name = gtk_entry_get_text(GTK_ENTRY(entries->name_entry));
    const char *doctor_age = gtk_entry_get_text(GTK_ENTRY(entries->age_entry));
    const char *doctor_specialization = gtk_entry_get_text(GTK_ENTRY(entries->specialization_entry));
    const char *doctor_contact = gtk_entry_get_text(GTK_ENTRY(entries->contact_entry));
    const char *doctor_experience = gtk_entry_get_text(GTK_ENTRY(entries->experience_entry));
    const char *doctor_fees = gtk_entry_get_text(GTK_ENTRY(entries->fees_entry));

    printf("Doctor Registration Details:\n");
    printf("Name: %s\n", doctor_name);
    printf("Age: %s\n", doctor_age);
    printf("Specialization: %s\n", doctor_specialization);
    printf("Contact: %s\n", doctor_contact);
    printf("Experience: %s\n", doctor_experience);
    printf("Fees: %s\n", doctor_fees);

    // Save the doctor data
    strcpy(doctors[doctor_count].name, doctor_name);
    strcpy(doctors[doctor_count].age, doctor_age);
    strcpy(doctors[doctor_count].specialization, doctor_specialization);
    strcpy(doctors[doctor_count].contact, doctor_contact);
    strcpy(doctors[doctor_count].experience, doctor_experience);
    strcpy(doctors[doctor_count].fees, doctor_fees);
    doctor_count++;

    GtkWidget *message_dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Doctor Registered Successfully!");
    gtk_dialog_run(GTK_DIALOG(message_dialog));
    gtk_widget_destroy(message_dialog);

    // Free the allocated memory for the entries structure
    g_free(entries);
}

int main(int argc, char *argv[]) {
    GtkWidget *window, *grid, *username_entry, *password_entry, *login_button, *error_label;
    GtkWidget *entries[4];

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Login");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Username entry
    username_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Username:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), username_entry, 1, 0, 2, 1);

    // Password entry
    password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Password:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), password_entry, 1, 1, 2, 1);

    // Error label
    error_label = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(grid), error_label, 0, 2, 3, 1);

    // Login button
    login_button = gtk_button_new_with_label("Login");
    gtk_grid_attach(GTK_GRID(grid), login_button, 0, 3, 3, 1);

    entries[0] = username_entry;
    entries[1] = password_entry;
    entries[2] = error_label;
    entries[3] = window;

    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_button_clicked), entries);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}