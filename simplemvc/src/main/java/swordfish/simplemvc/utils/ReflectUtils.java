package swordfish.simplemvc.utils;

import android.os.Parcelable;

import org.json.JSONArray;
import org.json.JSONObject;

import java.io.Serializable;
import java.lang.annotation.Annotation;
import java.lang.reflect.Field;
import java.lang.reflect.ParameterizedType;
import java.util.ArrayList;
import java.util.List;

import android.util.Log;

/**
 * Created by Fish on 16/7/20 in YiZhiBo.
 */
public class ReflectUtils {

    static List<Field> mCollectedFields = new ArrayList<>();

    /**
     * Collect a object's declared fields .
     *
     * @param fieldsList
     * @param obj
     * @param annotationClz
     * @param finalParentClz
     * @return The arrayList collected. Note that this arrayList is designed into single instance for
     * Memory sake , do clone to your local .
     */
    public static List<Field> collectField(List<Field> fieldsList, Object obj, Class<? extends Annotation> annotationClz, Class finalParentClz) {
        if (fieldsList == null) {
            fieldsList = mCollectedFields;
        }
        fieldsList.clear();

        Class<?> cls = obj.getClass();
        do {
            Field fields[] = cls.getDeclaredFields();
            for (Field f : fields) {

                if (annotationClz != null) {
                    if (f.getAnnotation(annotationClz) != null) {
                        fieldsList.add(f);
                    }
                } else {
                    fieldsList.add(f);
                }
            }
        } while ((cls = cls.getSuperclass()) != finalParentClz);
        return fieldsList;
    }

    public static List<Field> collectField(Object obj, Class<? extends Annotation> annotationClz, Class finalParentClz) {
        return collectField(null, obj, annotationClz, finalParentClz);
    }

    public static List<Field> collectField(Object obj, Class<? extends Annotation> annotationClz) {
        return collectField(null, obj, annotationClz, null);
    }


    public interface IDealWithField {

        void onDealInteger(String fieldName, Object valueObject, Field field);

        void onDealLong(String fieldName, Object valueObject, Field field);

        void onDealShort(String fieldName, Object valueObject, Field field);

        void onDealBoolean(String fieldName, Object valueObject, Field field);

        void onDealFloat(String fieldName, Object valueObject, Field field);

        void onDealDouble(String fieldName, Object valueObject, Field field);

        void onDealString(String fieldName, Object valueObject, Field field);

        void onDealJSONObject(String fieldName, Object valueObject, Field field);

        void onDealJSONArrayObject(String fieldName, Object valueObject, Field field);

        void onDealParcelable(String fieldName, Object valueObject, Field field);

        void onDealSerializable(String fieldName, Object valueObject, Field field);

        void onDealArrayList(String fieldName, Object valueObject, Field field);
    }

    public interface IDealWithFieldPro extends IDealWithField {

    }


    static final String TAG = ReflectUtils.class.getSimpleName();


    static boolean DEBUG = false;

    public static void dealWithField(List<Field> fieldList, Object fieldClassObject, IDealWithField handle) {
        int length = fieldList.size();
        for (int i = 0; i < length; i++) {
            Field field = fieldList.get(i);
            String fieldName = field.getName();
            field.setAccessible(true);
            try {
                Object fieldValueObject = field.get(fieldClassObject);
                Class<?> clzType = field.getType();
                if (clzType.isPrimitive()) {
                    if (clzType == short.class || clzType == Short.class) {
                        handle.onDealShort(fieldName, fieldValueObject, field);
                    } else if (clzType == int.class || clzType == Integer.class) {
                        handle.onDealInteger(fieldName, fieldValueObject, field);
                    } else if (clzType == long.class || clzType == Long.class) {
                        handle.onDealLong(fieldName, fieldValueObject, field);
                    } else if (clzType == boolean.class || clzType == Boolean.class) {
                        handle.onDealBoolean(fieldName, fieldValueObject, field);
                    } else if (clzType == double.class || clzType == Double.class) {
                        handle.onDealDouble(fieldName, fieldValueObject, field);
                    } else if (clzType == float.class || clzType == Float.class) {
                        handle.onDealFloat(fieldName, fieldValueObject, field);
                    } else {
                        throwUnDealFieldException(field);
                    }
                } else if (clzType == String.class) {
                    handle.onDealString(fieldName, fieldValueObject, field);
                } else if (clzType == JSONObject.class) {
                    handle.onDealJSONObject(fieldName, fieldValueObject, field);
                } else if (clzType == JSONArray.class) {
                    handle.onDealJSONArrayObject(fieldName, fieldValueObject, field);
                } else if (clzType == ArrayList.class) {
                    handle.onDealArrayList(fieldName, fieldValueObject, field);
                } else if (Parcelable.class.isAssignableFrom(clzType)) {
                    handle.onDealParcelable(fieldName, fieldValueObject, field);
                } else if (Serializable.class.isAssignableFrom(clzType)) {
                    handle.onDealSerializable(fieldName, fieldValueObject, field);
                } else if (List.class.isAssignableFrom(clzType)) {
                    throwUnDealFieldException(field);
                } else {
                    throwUnDealFieldException(field);
                }
            } catch (IllegalAccessException e) {
                e.printStackTrace();
                throw new RuntimeException(e);
            } catch (IllegalArgumentException e) {
                e.printStackTrace();
                throw new RuntimeException(e);
            }
        }

    }

    private static void throwUnDealFieldException(Field field) {
        throw new RuntimeException("\t can not deal -> " + field.getName() + " " + field.getClass());
    }

    private static void dealWithFiledList(Field field, Object object) {
        ParameterizedType pt = (ParameterizedType) field.getGenericType();
        Class myListTypeClz = (Class) pt.getActualTypeArguments()[0];
        if (DEBUG) Log.v(TAG, " myListTypeClz -> " + myListTypeClz.getName());

        if (myListTypeClz == short.class || myListTypeClz == Short.class) {
        } else if (myListTypeClz == int.class || myListTypeClz == Integer.class) {
        } else if (myListTypeClz == long.class || myListTypeClz == Long.class) {
        } else if (myListTypeClz == boolean.class || myListTypeClz == Boolean.class) {
        } else if (myListTypeClz == double.class || myListTypeClz == Double.class) {
        } else if (myListTypeClz == float.class || myListTypeClz == Float.class) {
        } else {
        }
    }

}
